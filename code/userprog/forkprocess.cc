#include "forkprocess.h"
#include "system.h"


void StartForkedProcess(int arg) {
    currentThread->space->InitRegisters ();
    currentThread->space->RestoreState();
    currentThread->space->InitMainThread();
    machine->Run();
}

int do_ForkExec (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL) {
        printf ("Unable to open file %s\n", filename);
        delete [] filename;
        return -1;
    }

    space = new AddrSpace (executable);

    if (space == NULL) {
        printf("%s : Insufficient memory to start the process.\n",
                     filename);
        delete executable;
        delete [] filename;
        return -1;
    }

    delete executable;


    Thread * mainThread = new Thread(filename);
    mainThread->space = space;
//    mainThread->space->InitRegisters();
    machine->UpdateRunningProcess(1); // appel atomique
    mainThread->ForkProcess (StartForkedProcess, 0);

    return 0;
}

void do_Exit() {
//    printf("ExitProcess : %s", currentThread->getName());
    DEBUG('p', "ExitProcess : %s", currentThread->getName());
    machine->UpdateRunningProcess(-1);
    if (machine->Alone()) {
        interrupt->Halt();
    }
    currentThread->Finish();
}

