#include "forkprocess.h"
#include "system.h"

void StartForkedProcess(int arg) {
    currentThread->space->RestoreState();
    currentThread->space->InitRegisters ();
    currentThread->space->InitMainThread();
    machine->semThreadFork->V();
    machine->Run();
}

int do_ForkExec (char *filename)
{
    machine->semThreadFork->P();
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL) {
        printf ("Unable to open file %s\n", filename);
        delete [] filename;
        return -1;
    }
    // Creation d'un nouvel espace d'adressage
    space = new AddrSpace (executable);

    // Si c'est null on arrete la
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

    machine->UpdateRunningProcess(1); // appel atomique
    mainThread->Fork (StartForkedProcess, 0);

    return 0;
}

void do_Exit() {
    DEBUG('p', "ExitProcess : %s", currentThread->getName());
    machine->UpdateRunningProcess(-1);
    if (machine->Alone()) {
        interrupt->Halt();
    }
//    AddrSpace *space = currentThread->space;
//    delete currentThread->space;
    currentThread->Finish();
}

