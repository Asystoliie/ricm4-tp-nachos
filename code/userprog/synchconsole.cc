#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"


static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *mutex;

static void ReadAvail(int arg) {
    readAvail->V();
}

static void WriteDone(int arg) {
    writeDone->V();
}

SynchConsole::SynchConsole(char *readFile, char *writeFile) {
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    mutex = new Semaphore("mutex", 1);
    console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole() {
    delete console;
    delete writeDone;
    delete readAvail;
    delete mutex;
}

void SynchConsole::SynchPutChar(const char ch) {
    /* On écrit un char on se bloque en attendant que la console appelle
     * (WriteDone>V())
     */
    console->PutChar(ch);
    writeDone->P();
}

char SynchConsole::SynchGetChar() {
    /* Lorsqu'il y a rien à lire, on se bloque, et dès qu'il y a quelques chose
     * à lire, on sait qu'on sera débloqué (ReadAvail->V())
     */
    readAvail->P();
    return console->GetChar();
}

void SynchConsole::SynchPutString(const char string[]) {
    /* On utilise un mutex pour que les appels SynchPutString soient atomiques
     * C'est à dire que deux appels à SynchPutString() affichent correctement
     * les chaines de caractères...
     * * */
    mutex->P();
    for(int i=0; i<MAX_STRING_SIZE-1;i++) {
        if(string[i] == '\0')
            break;
        this->SynchPutChar(string[i]);
    }
    mutex->V();
}

void SynchConsole::SynchGetString(char *buffer, int n) {
  /* On utilise un mutex pour que tous les appels SynchGetString soient
   * atomiques.
   * * */
  int i;
  char c;
  mutex->P();
  for (i=0; i<n-1; i++) {
    c = this->SynchGetChar();
    // CTRL+D pour arrêter la saisie
    if(c == EOF)
      break;
    else
      buffer[i] = c;
  }
  buffer[i] = '\0';
  mutex->V();
}
