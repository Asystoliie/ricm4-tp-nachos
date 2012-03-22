#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"


static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *writeMutex;
static Semaphore *readMutex;
static Semaphore * putStringMutex;

static void ReadAvail(int arg) {
    readAvail->V();
}

static void WriteDone(int arg) {
    writeDone->V();
}

SynchConsole::SynchConsole(char *readFile, char *writeFile) {
    readAvail  = new Semaphore("read avail", 0);
    writeDone  = new Semaphore("write done", 0);
    writeMutex = new Semaphore("writeMutex", 1);
    readMutex  = new Semaphore("readMutex", 1);
    putStringMutex = new Semaphore("putStringMutex", 1);
    console    = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole() {
    delete console;
    delete writeDone;
    delete readAvail;
    delete writeMutex;
    delete readMutex;
}

void SynchConsole::SynchPutChar(const char ch) {
    /* On écrit un char on se bloque en attendant que la console appelle
     * (WriteDone>V())
     */
    writeMutex->P();
    console->PutChar(ch);
    writeDone->P();
    writeMutex->V();
}

char SynchConsole::SynchGetChar() {
    /* Lorsqu'il y a rien à lire, on se bloque, et dès qu'il y a quelques chose
     * à lire, on sait qu'on sera débloqué (ReadAvail->V())
     */
    readMutex->P();
    readAvail->P();
    return console->GetChar();
    readMutex->V();
}

void SynchConsole::SynchPutString(const char string[]) {
    /* On utilise un mutex pour que les appels SynchPutString soient atomiques
     * C'est à dire que deux appels à SynchPutString() affichent correctement
     * les chaines de caractères...
     * * */
    putStringMutex->P();
    for(int i=0; i<MAX_STRING_SIZE-1;i++) {
        if(string[i] == '\0')
            break;
        this->SynchPutChar(string[i]);
    }
    putStringMutex->V();
}

void SynchConsole::SynchGetString(char *buffer, int n, char delim) {
  /* On utilise un mutex pour que tous les appels SynchGetString soient
   * atomiques.
   * * */
  int i;
  char c;
  for (i=0; i<n-1; i++) {
    c = this->SynchGetChar();
    // CTRL+D pour arrêter la saisie
    if(c == delim)
      break;
    else
      buffer[i] = c;
  }
  buffer[i] = '\0';
}


void SynchConsole::SynchGetString(char *buffer, int n) {
  this->SynchGetString(buffer, n, EOF);
}

void SynchConsole::SynchPutInt(int value) {
  char * buffer = new char[MAX_STRING_SIZE];
  // on ecrit dans le buffer la valeur avec sprintf
  snprintf(buffer,MAX_STRING_SIZE, "%d", value);
  this->SynchPutString(buffer);
  delete [] buffer;
}

int SynchConsole::SynchGetInt() {
  int value;
  char * buffer = new char[MAX_STRING_SIZE];
  this->SynchGetString(buffer, MAX_STRING_SIZE, '\n');
  sscanf(buffer, "%d", &value);
  delete [] buffer;
  return value;
}

