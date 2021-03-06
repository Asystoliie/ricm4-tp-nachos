#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"

class SynchConsole {
    public:
        SynchConsole(char *readFile, char *writeFile);
        ~SynchConsole();
        void SynchPutChar(const char ch);
        char SynchGetChar();
        // Unix putchar(3S)
        // Unix getchar(3S)
        void SynchPutString(char *s); // Unix puts(3S)
        void SynchGetString(char *s, int n);
        void SynchGetString(char *s, int n, char delim);
        void SynchPutInt(int value);
        int SynchGetInt();
        Semaphore * putStringMutex;
        // Unix fgets(3S)
    private:
        Console *console;
};
#endif // SYNCHCONSOLE_H

