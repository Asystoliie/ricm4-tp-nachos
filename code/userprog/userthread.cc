#include "userthread.h"

void StartUserThread(int thread) {
    UserThread *t = (UserThread *) thread;
    // L'id du thread informe egalement le numéro de page du thread
    currentThread->space->InitThreadRegisters(t->func, t->arg, t->getId());
    currentThread->space->UpdateRunningThreads(1); // appel atomique
    machine->Run();
}

UserThread::UserThread(const char *debugName, int f, int a, int callback) : Thread(debugName) {
    this->func = f;
    this->arg = a;
    // A la fin du thread on appel cette nouvelle fonction
    this->UpdateCallBackRegister(callback);
}

void UserThread::Fork () {
    DEBUG ('t', "Forking userThread \"%s\"\n", getName ());
    Thread::Fork (StartUserThread, (int) this);
}

void UserThread::UpdateCallBackRegister(int value) {
    this->userRegisters[31] = value;
}

int do_UserThreadCreate(int f, int arg, int callback) {
    UserThread* newThread = new UserThread((char*)f, f, arg, callback);
    if (newThread == NULL) { return -1; }
    currentThread->space->semStackBitMap->P();
    newThread->setId(currentThread->space->stackBitMap->Find());
    currentThread->space->semStackBitMap->V();


    if (newThread->getId() < 0) { return 0; }

    newThread->Fork();
    return newThread->getId();
}

void do_UserThreadExit() {
    currentThread->space->UpdateRunningThreads(-1); // appel atomique
    // Je suis le main donc je vais attendre les autres
    if (currentThread->getId() == 0) {
        currentThread->space->semWaitThreads->P();
        interrupt->Halt();
    }
    // Sinon je suis qu'un misérable userthread et ma vie se termine ici...

    // On libere les threads en attente sur moi
    currentThread->space->semJoinThreads[currentThread->getId()]->V();
    // Dans notre implementation plusieurs thread ne peuvent pas attendre un meme
    // thread, car un seul sera liberé (instruction ci dessus)
    // Facilement amélirable nottament en creant une chaine de thread qui se libere
    // Mais on a des soucis pour pouvoir reutilisé ce sémaphore, car il y aura
    // au final toujours 1 jeton au lieu de 0, et donc plus aucun thread ne peut
    // attendre le prochain thread a hériter de la zone

    currentThread->space->FreeBitMap(); // appel atomique

    currentThread->Finish();
}


int do_UserThreadJoin(int thread_id) {
    ASSERT(thread_id > 0);
    if (currentThread->space->stackBitMap->Test(thread_id)) {
        // On attends que le thread se termine
        currentThread->space->semJoinThreads[thread_id]->P();
        return 0;
    }
    return -1;
}
