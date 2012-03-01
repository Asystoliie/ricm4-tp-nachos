#include "userthread.h"


void StartUserThread(int parameters){
    UserThreadParameters *p = (UserThreadParameters *) parameters;
    currentThread->space->InitRegisters(p->f, p->arg);
    machine->Run();
    return;
}

UserThread::UserThread(const char *debugName, int f, int arg) : Thread(debugName) {
    // On encapsule la fonction et les parametres dans notre structure parameters
    this->id = 0;
    this->parameters = new UserThreadParameters;
    parameters->f = f;
    parameters->arg = arg;

}

UserThread::~UserThread() {
    delete parameters;
}

void UserThread::StartThread(void) {
    this->Fork(StartUserThread, (int) this->parameters);
}

int do_UserThreadCreate(int f, int arg) {


    UserThread* newThread = new UserThread("Test", f, arg);
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
