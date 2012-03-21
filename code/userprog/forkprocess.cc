#include "forkprocess.h"
#include "system.h"


int ForkExec (char *filename)
{
    int pid = machine->GetNewPID();

    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL) {
        printf ("Unable to open file %s\n", filename);
        return -1;
    }

    space = new AddrSpace (executable);

    if (space == NULL) {
        printf("%s : Memoire insuffisante pour charger l'executable.\n", filename);
        delete (executable);
        delete (space);
        return -1;
    }
    delete executable;        // close file

    currentThread->space->SaveState();
    currentThread->SaveUserState();

    currentThread->space = space;
    currentThread->Fork (StartProcess, 0);

    currentThread->RestoreUserState();
    currentThread->space->RestoreState();
    return pid;
}

void StartProcess(int arg) {
    currentThread->space->InitRegisters();
    machine->UpdateRunningProcess(1); // appel atomique
    machine->Run();
}

