#include "userthread.h"


int do_UserThreadCreate(int f, int arg){

    // On encapsule la fonction et les parametres dans notre structure parameters
    UserThreadParameters *parameters = new UserThreadParameters;
    parameters->f = f;
    parameters->arg = arg;

    Thread* newThread = new Thread((char *)f);
    if (newThread == NULL){
        printf("Failed to create new Thread : newThread is NULL\n");
        return -1;
    }
    newThread->Fork(StartUserThread, (int)parameters);

    return (int) newThread;
}

void do_UserThreadExit() {
    currentThread->Finish();
}

void StartUserThread(int parameters){
    UserThreadParameters *p = (UserThreadParameters *) parameters;
    currentThread->space->InitRegisters(p->f, p->arg);
    machine->Run();
    return;
}

