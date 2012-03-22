#include "forkprocess.h"
#include "system.h"


int do_ForkExec (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL) {
        printf ("Unable to open file %s\n", filename);
        return -1;
    }

    space = new AddrSpace (executable);

    if (space == NULL) {
        printf("%s : Memoire insuffisante pour le lancer le processus.\n",
                     filename);
        delete (executable);
//        delete (space);
        return -1;
    }

    delete executable;

    Thread * mainThread = new Thread(filename);
    mainThread->space = space;

    mainThread->Fork (StartForkedProcess, 0);
    return 0;
}

void do_Exit() {
    if (machine->Alone()) {
        interrupt->Halt();
    }
    AddrSpace * space = currentThread->space;
    delete space;
    machine->UpdateRunningProcess(-1);
    currentThread->Finish();
}

void StartForkedProcess(int arg) {
    currentThread->space->InitRegisters();
    machine->UpdateRunningProcess(1); // appel atomique
    machine->Run();
}

