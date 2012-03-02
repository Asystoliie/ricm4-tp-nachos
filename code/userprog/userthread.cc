#include "userthread.h"

void StartUserThread(int args){
    UserThreadArgs *p = (UserThreadArgs *) args;
    currentThread->space->InitThreadRegisters(p->f, p->arg, currentThread->getId());
    machine->Run();
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
    int numPage = newThread->space->stackBitMap->Find();
    currentThread->space->semStackBitMap->V();

    if (numPage < numPage) {
        printf("Failed to create new Thread : numPage = %d\n", numPage);
        return -1;
    }
    newThread->setId(numPage);
    newThread->StartThread();
    return (int) newThread;
}

void do_UserThreadExit() {
    currentThread->Finish();
}

