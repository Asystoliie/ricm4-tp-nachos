#include "forkprocess.h"
#include "system.h"

void StartForkedProcess(int arg) {
    currentThread->space->RestoreState();
    currentThread->space->InitRegisters ();
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
    // Creation d'un nouvel espace d'adressage
    space = new AddrSpace (executable);

    // Si c'est null ou qu'il n'y a pas assez de memoire on arrete la
    if (space == NULL || !space->AvailFrames) {
        printf("%s : Insufficient memory to start the process.\n",
                     filename);
        delete executable;
        delete [] filename;
        return -1;
    }
    delete executable;

    // Creation du nouveau thread main du nouveau processus
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

    // /!\ Provoque une erreur
    // delete currentThread->space;
    // On libere les frames c'est la fin du processus
    currentThread->space->ReleaseFrames();
    currentThread->Finish();
}

