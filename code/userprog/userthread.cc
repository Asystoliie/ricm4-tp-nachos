#include "userthread.h"

void StartUserThread(int args){
    UserThreadArgs *p = (UserThreadArgs *) args;
    currentThread->space->InitThreadRegisters(p->f, p->arg, currentThread->getId());
    machine->Run();
    currentThread->space->RestoreState();
    return;
}

UserThread::UserThread(const char *debugName, int f, int arg) : Thread(debugName) {
    // On encapsule la fonction et les parametres dans notre structure parameters
    this->args = new UserThreadArgs;
    args->f = f;
    args->arg = arg;
}

UserThread::~UserThread() {
    delete args;
}

void UserThread::StartThread(void) {
    this->setId(0);
    this->Fork(StartUserThread, (int) this->args);
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
        printf("Failed to create new Thread : numPage = %d\n", newThread->getId());
        return -1;
    }
    newThread->StartThread();
    return newThread->getId();
}

void do_UserThreadExit() {
    currentThread->space->semStackBitMap->P();
    currentThread->space->stackBitMap->Clear(currentThread->getId());
    currentThread->space->semStackBitMap->V();
    currentThread->Finish();
}

