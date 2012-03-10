//#include "system.h"
#include "syscall.h"

void print(int i) {
    int j = 0;
    for (j=0; j<12000; j++) {}
    PutInt(i);PutChar('\n');
/*    UserThreadExit(); // Facultatif*/
}

int main() {
    PutString("Test de thread : \n");
    int i = 0;
    int thread_id;
    for (i=0; i<90; i++) {
        thread_id = UserThreadCreate(print, (void *)i);
        if (thread_id == 0) {
            PutString("Impossible de créer de nouveaux Thread\n");
            break;
        } else if (thread_id == -1) {
            PutString("Erreur UserThreadCreate\n");
            break;
        } else {
            PutString("Thread : ");PutInt(i);PutString(" OK\n");
        }
    }
    PutString("Fin du main : \n");
    return 0;
}

