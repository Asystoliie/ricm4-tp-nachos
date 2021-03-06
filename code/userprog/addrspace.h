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
#include "synch.h"

#define UserStackSize        4096// increase this as necessary! (4k)
#define UserThreadNumPage    3 // 3 pages par thread


class AddrSpace
{
    public:
        AddrSpace (OpenFile * executable);
        // Create an address space,
        // initializing it with the program
        // stored in the file "executable"
        ~AddrSpace ();
        // De-allocate an address space

        // nombre de threads max
        const static int userMaxNumThread = (int) (UserStackSize / (UserThreadNumPage * PageSize));

        void InitRegisters ();
        // Initialize user-level CPU registers,
        // before jumping to user code

        void SaveState ();
        // Save/restore address space-specific
        void RestoreState ();
        // info on a context switch

        // Initialize user-level CPU registers,  before jumping to user
        // function f(arg)
        void InitThreadRegisters (int f, int arg, int thread_id);

        // Le nombre de thread en cours d'executions (à protéger par un mutex)
        int runningThreads;
        // l'objet bitmap qui permet de trouver les zones libres pour les
        // nouveaux threads sans devoir gérer ça nous meme..
        BitMap *stackBitMap;
        // Pour manipuler la variable runningThreads
        Semaphore *semRunningThreads;
        // Pour manipuler la bitmap
        Semaphore *semStackBitMap;
        // Pour permettre a un ou plusieurs threads de se bloquer en attendant
        // qu'un autre se termine
        Semaphore *semJoinThreads[userMaxNumThread];

        // Ces methodes permettent de manipuler les variables à protéger d'une
        // utilisation multithread
        void UpdateRunningThreads(int i);
        // Permet de savoir si je suis le dernier thread
        int Alone();
        int GetNewZone();
        void FreeBitMap();

        // Permet de compter le nombre total de thread et donc d'avoir des ids
        // unique pour les threads
        int countThreads;
        // Ce tableau fait le mappage entre thread_id et numéro de la zone
        // correspondant à ce thread dans la pile
        int *threadZoneMap;
        // Pour manipuler les deux bitmap
        Semaphore *semThreadZoneMap;

        // Methodes qui permettent de manipuler les deux attributs précédent
        int GetNewThreadId(int zone);
        void RemoveId(int zone);
        int GetZoneFromThreadId(int thread_id);

        // Permet d'initialiser le thread main
        void InitMainThread();
        void ReleaseFrames();
        bool AvailFrames;
        bool ToBeDestroyed;

    private:
        TranslationEntry * pageTable;
        // Assume linear page table translation
        // for now!
        unsigned int numPages;
        // Number of pages in the virtual
        // address space
};

#endif // ADDRSPACE_H

