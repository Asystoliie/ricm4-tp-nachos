#include "userthread.h"

void StartUserThread(int args){
    UserThreadArgs *p = (UserThreadArgs *) args;
    currentThread->space->InitRegisters(p->f, p->arg);
    machine->Run();
    return;
}

UserThread::UserThread(const char *debugName, int f, int arg) : Thread(debugName) {
    // On encapsule la fonction et les parametres dans notre structure parameters
    this->id = 0;
    this->args = new UserThreadArgs;
    args->f = f;
    args->arg = arg;
}

UserThread::~UserThread() {
    delete args;
}

void UserThread::StartThread(void) {
    this->Fork(StartUserThread, (int) this->args);
}

int do_UserThreadCreate(int f, int arg) {
    UserThread* newThread = new UserThread((char*)f, f, arg);
    if (newThread == NULL) {
        printf("Failed to create new Thread : newThread is NULL\n");
        return -1;
    }
    newThread->StartThread();

    return (int) newThread;
}

void do_UserThreadExit() {
    currentThread->Finish();
}
