#include "userthread.h"


int do_UserThreadCreate(int f, int arg){

    // On encapsule la fonction et les parametres dans notre structure parameters
    UserThreadParameters *parameters = new UserThreadParameters;
    parameters->f = f;
    parameters->arg = arg;

    int thread_ID = currentThread->space->stackBitMap->Find();

    if (thread_ID == -1){
        printf("Fail to create new thread : thread_ID == -1\n");
        return -1;
    }

    Thread* newThread = new Thread((char *)f);
    if (newThread == NULL){
        printf("Failed to create new Thread : newThread is NULL\n");
        return -1;
    }
    newThread->thread_ID = thread_ID;
    newThread->Fork(StartUserThread, (int)parameters);

    return newThread->thread_ID;
}

void do_UserThreadExit(){
    // On ne kill pas le thread main
    if (currentThread->thread_ID != 0)
        currentThread->Finish();
}

void StartUserThread(int parameters){
    UserThreadParameters *p = (UserThreadParameters *) parameters;
    currentThread->space->InitRegisters(p->f, p->arg);
    machine->Run();
    return;
}

