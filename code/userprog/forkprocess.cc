#include "forkprocess.h"
#include "system.h"


//    OpenFile *executable = fileSystem->Open (filename);
//    AddrSpace *space;

//    if (executable == NULL)
//      {
//	  printf ("Unable to open file %s\n", filename);
//	  return;
//      }
//
//      #ifdef CHANGED
//    synchconsole = new SynchConsole (NULL, NULL);
//	#endif // CHANGED
//
//    space = new AddrSpace (executable);
//    currentThread->space = space;

//    delete executable;		// close file

//    space->InitRegisters ();	// set the initial register values
//    space->RestoreState ();	// load page table register

//    machine->Run ();		// jump to the user progam
//    ASSERT (FALSE);		// machine->Run never returns;
//    // the address space exits
//    // by doing the syscall "exit"

