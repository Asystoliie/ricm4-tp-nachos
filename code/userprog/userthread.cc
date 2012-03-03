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
    currentThread->space->FreeBitMap(); // appel atomique

    // Je suis le main donc je vais attendre les autres
    if (currentThread->getId() == 0) {
        currentThread->space->semWaitThreads->P();
        interrupt->Halt();
    }
    // Sinon je suis qu'un misérable userthread et ma vie se termine ici...
    currentThread->Finish();
}
