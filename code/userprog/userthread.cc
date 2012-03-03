#include "userthread.h"

void StartUserThread(int thread) {
    UserThread *t = (UserThread *) thread;
    // L'id du thread informe egalement le numéro de page du thread
    currentThread->space->InitThreadRegisters(t->func, t->arg, t->getId());
    currentThread->space->UpdateRunningThreads(1); // appel atomique
    machine->Run();
}

UserThread::UserThread(const char *debugName, int f, int a) : Thread(debugName) {
    this->func = f;
    this->arg = a;
}

void UserThread::Fork () {
    DEBUG ('t', "Forking userThread");
    Thread::Fork (StartUserThread, (int) this);
}

int do_UserThreadCreate(int f, int arg) {
    UserThread* newThread = new UserThread((char*)f, f, arg);
    if (newThread == NULL) {
        printf("Failed to create new Thread : newThread is NULL\n");
        return -1;
    }
    currentThread->space->semStackBitMap->P();
    newThread->setId(currentThread->space->stackBitMap->Find());
    currentThread->space->semStackBitMap->V();

    if (newThread->getId() < 0) {
        printf("Failed to create new Thread : numPage < 0");
        return -1;
    }

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
    currentThread->Finish();
}
