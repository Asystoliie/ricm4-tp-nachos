// addrspace.cc
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

#include <strings.h>        /* for bzero */

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
    WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

void ReadAtVirtual( OpenFile *executable, int virtualaddr, int numBytes,
                    int position, TranslationEntry *pageTable,
                    unsigned int numPages ) {
    /* Lecture depuis la mémoire virtuelle
    * On commence par initialisé les table de pages dans la machine
    * Ensuite on lit a partir de mémoire physique pour recopie octet
    * par octet dans la mémoire virtuelle (par un buffer par sécurité)
    */
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
    //buffer to read the specified portion of executable
    char * buffer = new char[numBytes];
    // On lit au plus numBytes octets
    int nb_read = executable->ReadAt(buffer, numBytes, position);
    // On ecrit dans la mémoire virtuelle
    for (int i = 0; i < nb_read; i++)
        machine->WriteMem(virtualaddr+i, 1, buffer[i]);

}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
    (WordToHost (noffH.noffMagic) == NOFFMAGIC))
    SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

    // how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
    // we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    ASSERT (numPages <= NumPhysPages);
    // check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory

    // Le nombre de thread en cours d'executions (à protéger par un mutex)
    this->runningThreads = 1; // 1 = le thread main//currentThread
    // l'objet bitmap qui permet de trouver les zones libres pour les
    // nouveaux threads, sans devoir gérer ça nous meme..
    // Ici on gere X zones de `UserThreadNumPage` pages
    this->stackBitMap = new BitMap(this->userMaxNumThread);
    // Mutex pour manipuler la variable running_threads
    this->semRunningThreads = new Semaphore("semRunningThreads", 1);
    // Permet de protéger la bitmap
    this->semStackBitMap = new Semaphore("semStackBitMap", 1);

    // On les initialise tous à 1 jeton
    for (int j = 0; j<this->userMaxNumThread ; j++) {
        this->semJoinThreads[j] = new Semaphore("semJoinThread ", 1);
    }

    // Mise en place du tableau de mappage entre thread_ids et numéro de zone
    this->countThreads = 0;
    this->threadZoneMap = new int[this->userMaxNumThread];
    for(int j=0; j<this->userMaxNumThread; j++) {
        this->threadZoneMap[j] = -1;
    }

    this->semThreadZoneMap = new Semaphore("threadZoneMap", 1);

    // La zone 0 est pour le thread main
    int zone = this->stackBitMap->Find();
    currentThread->setZone(zone);
    currentThread->setId(this->GetNewThreadId(zone));

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
       numPages, size);

    // first, set up the translation
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) {
        pageTable[i].virtualPage = i;
        // for now, virtual page # = phys page #
        pageTable[i].physicalPage = i+1;
        pageTable[i].valid = TRUE;
        pageTable[i].use = FALSE;
        pageTable[i].dirty = FALSE;
        pageTable[i].readOnly = FALSE;
        // if the code segment was entirely on
        // a separate page, we could set its
        // pages to be read-only
    }


    // zero out the entire address space, to zero the unitialized data segment
    // and the stack segment
    bzero (machine->mainMemory, size);

    // then, copy in the code and data segments into memory
    if (noffH.code.size > 0) {
        DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
            noffH.code.virtualAddr, noffH.code.size);
        ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size,
            noffH.code.inFileAddr, pageTable, numPages);
    }
    if (noffH.initData.size > 0) {
        DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
            noffH.initData.virtualAddr, noffH.initData.size);
        ReadAtVirtual(executable, noffH.initData.virtualAddr,
          noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
    }
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  // LB: Missing [] for delete
  // delete pageTable;
  delete [] pageTable;
  delete [] threadZoneMap;
  delete stackBitMap;
  delete semRunningThreads;
  delete semStackBitMap;
  delete [] semJoinThreads;
  delete semThreadZoneMap;
  // End of modification
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
    machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to %d\n",
       numPages * PageSize - 16);
}


void AddrSpace::InitThreadRegisters (int f, int arg, int thread_zone)
{
    machine->WriteRegister (PCReg, f);
    machine->WriteRegister (NextPCReg, f+4);
    // On ajoute l'arguments
    machine->WriteRegister (4, arg);
    // On se place sur la pile du thread
    int threadOffset = UserThreadNumPage * PageSize  * thread_zone;
    machine->WriteRegister (StackReg, numPages * PageSize - 16 - PageSize - threadOffset);

    DEBUG ('a', "Initializing thread stack register to %d\n",
       numPages * PageSize - 16 - threadOffset);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}


void AddrSpace::UpdateRunningThreads(int value) {
    ASSERT (value != 1 || value !=-1);
    this->semRunningThreads->P();
    this->runningThreads += value;
    DEBUG ('t', "runningThread =  %d\n", runningThreads);
    this->semRunningThreads->V();
}

int AddrSpace::Alone() {
    int value = 0;
    this->semRunningThreads->P();
    if (this->runningThreads == 0)
        value = 1;
    this->semRunningThreads->V();
    return value;
}

int AddrSpace::GetNewZone() {
    int zone;
    this->semStackBitMap->P();
    zone = this->stackBitMap->Find();
    this->semStackBitMap->V();
    return zone;
}

void AddrSpace::FreeBitMap() {
    this->semStackBitMap->P();
    // On libere la zone
    this->stackBitMap->Clear(currentThread->getZone());
    this->RemoveId(currentThread->getZone());
    this->semStackBitMap->V();
}

int AddrSpace::GetNewThreadId(int zone) {
    this->semThreadZoneMap->P();
    int id = this->countThreads;
    threadZoneMap[zone]=id;
    this->countThreads++;
    this->semThreadZoneMap->V();
    return id;
}

int AddrSpace::GetZoneFromThreadId(int thread_id) {
    int zone = -1;
    this->semThreadZoneMap->P();
    for(int j = 0; j<userMaxNumThread; j++) {
        if (threadZoneMap[j] == thread_id) {
            if (this->stackBitMap->Test(j))
                zone = j;
            break;
        }
    }
    this->semThreadZoneMap->V();
    return zone;
}

void AddrSpace::RemoveId(int zone){
    this->semThreadZoneMap->P();
    threadZoneMap[zone]=-1;
    this->semThreadZoneMap->V();
}

