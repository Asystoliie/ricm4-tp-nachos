// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "bitmap.h"

#define UserStackSize        1024// increase this as necessary! (4k)
#define UserThreadNumPage    1

class Semaphore;

class AddrSpace
{
    public:
        AddrSpace (OpenFile * executable);
        // Create an address space,
        // initializing it with the program
        // stored in the file "executable"
        ~AddrSpace ();
        // De-allocate an address space

        int userMaxNumThread;

        void InitRegisters ();
        // Initialize user-level CPU registers,
        // before jumping to user code


        void SaveState ();        // Save/restore address space-specific
        void RestoreState ();    // info on a context switch

        // Initialize user-level CPU registers,  before jumping to user
        // function f(arg)
        void InitThreadRegisters (int f, int arg, int thread_id);

        // Le nombre de thread en cours d'executions (à protéger par un mutex)
        int runningThreads;
        // l'objet bitmap qui permet de trouver les zones libres pour les
        // nouveaux threads sans devoir gérer ça nous meme..
        BitMap *stackBitMap;
        // Mutex pour manipuler la variable running_threads
        Semaphore *semRunningThreads;
        Semaphore *semStackBitMap;
        // Pour la terminaison du main
        Semaphore *semAnyThreads;

    private:
        TranslationEntry * pageTable;    // Assume linear page table translation
        // for now!
        unsigned int numPages;    // Number of pages in the virtual
        // address space
};

#endif // ADDRSPACE_H

