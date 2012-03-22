#include "syscall.h"

void print(char c) {
    int j = 0, i=0;
    for (j=0; j<3; j++) {
        for (i=0; i<3000; i++) {}
        PutChar(c);
    }
/*    UserThreadExit(); // Facultatif*/
}

int main() {
    int i = 0;
    for (i=0; i<3000; i++) {}
    PutString("Début du fils 0 : lancement des deux threads a et z\n");
    int t1, t2;
    t1 = UserThreadCreate(print, (void *)'a');
    t2 = UserThreadCreate(print, (void *)'z');
    UserThreadJoin(t1);
    UserThreadJoin(t2);
    PutString("\nFin du thread main du fils 0\n");
    return 0;
}

