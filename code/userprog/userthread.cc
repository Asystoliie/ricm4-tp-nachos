#include "userthread.h"

void StartUserThread(int thread) {
    UserThread *t = (UserThread *) thread;
    // L'id du thread informe egalement le numéro de page du thread
    currentThread->space->InitThreadRegisters(t->func, t->arg, t->getZone());
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
    int zone = currentThread->space->stackBitMap->Find();
    int thread_id = currentThread->space->GetNewThreadId(zone);
    currentThread->space->semStackBitMap->V();

    if (zone < 0) { return 0; }
    if (thread_id < 0) { return 0; }

    newThread->setId(thread_id);
    newThread->setZone(zone);

    // Avant de commencer on prend le jetton, pour que tout thread qui appelle
    // userThreadJoin sur moi, est bloqué.
    currentThread->space->semJoinThreads[newThread->getZone()]->P();

    newThread->Fork();
    return newThread->getId();
}

void do_UserThreadExit() {
    currentThread->space->UpdateRunningThreads(-1); // appel atomique

    // On libere les threads en attente sur moi
    currentThread->space->semJoinThreads[currentThread->getZone()]->V();
    // Plusieurs threads peuvent attendre que je me termine.
    // Il faut donc que dans la fonction join, les thread en attente se
    // reveillent les uns les autres

    // On a plus besoin de la zone
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
    int zone = currentThread->space->GetZoneFromThreadId(thread_id);
    ASSERT(zone >= 0);
    if (currentThread->space->stackBitMap->Test(zone)) {
        // On attends que le thread se termine
        currentThread->space->semJoinThreads[zone]->P();
        // On reveille le suivant qui peut etre soit le prochain thread à qui
        // on a allouer la zone, soit un autre thread qui avait appeller join
        currentThread->space->semJoinThreads[zone]->V();
        return 0;
    }
    return -1;
}

