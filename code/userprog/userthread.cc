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

    // Avant de commencer on prend le jetton, pour que tout thread qui appelle
    // userThreadJoin sur moi, est bloqué.
    currentThread->space->semJoinThreads[newThread->getId()]->P();

    newThread->Fork();
    return newThread->getId();
}

void do_UserThreadExit() {
    currentThread->space->UpdateRunningThreads(-1); // appel atomique
    // Sinon je suis qu'un misérable userthread et ma vie se termine ici...

    // On libere les threads en attente sur moi
    currentThread->space->semJoinThreads[currentThread->getId()]->V();
    // Plusieurs threads peuvent attendre que je me termine.
    // Il faut donc que dans la fonction join, les thread en attente se
    // reveillent les uns les autres

    currentThread->space->FreeBitMap(); // appel atomique

    currentThread->space->semRunningThreads->P();
    if(currentThread->space->runningThreads == 0)
        // On libere le thread main est en train d'attendre...
        // Si je suis le thread main, je ne serrais alors pas bloqué plus tard
        interrupt->Halt();
    currentThread->space->semRunningThreads->V();

    currentThread->Finish();
}


int do_UserThreadJoin(int thread_id) {
    ASSERT(thread_id > 0);
    if (currentThread->space->stackBitMap->Test(thread_id)) {
        // On attends que le thread se termine
        currentThread->space->semJoinThreads[thread_id]->P();
        // On reveille le suivant qui peut etre soit le prochain thread à qui
        // on a allouer la zone, soit un autre thread qui avait appeller join
        currentThread->space->semJoinThreads[thread_id]->V();
        return 0;
    }
    return -1;
}

