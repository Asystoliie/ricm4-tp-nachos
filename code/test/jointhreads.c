//#include "system.h"
#include "syscall.h"

void print(int i) {
    int j = 0;
    for (j=0; j<120000; j++) {}
        PutInt(i);PutChar('\n');
/*    UserThreadExit(); // Facultatif*/
}

int main() {
    int thread_id;
    PutString("Je lance le Thread 1\n");
    thread_id = UserThreadCreate(print, (void *)1);
    if (thread_id > 0) {
        UserThreadJoin(thread_id);
    } else {
        PutString("Erreur UserThreadCreate\n");
    }
    PutString("Fin Thread 1\n");
    PutString("Je lance le Thread 2\n");
    thread_id = UserThreadCreate(print, (void *)2);
    if (thread_id > 0) {
        UserThreadJoin(thread_id);
    } else {
        PutString("Erreur UserThreadCreate\n");
    }
    PutString("Fin Thread 2\n");
    PutString("Je lance le Thread 3\n");
    thread_id = UserThreadCreate(print, (void *)3);
    if (thread_id > 0) {
        UserThreadJoin(thread_id);
    } else {
        PutString("Erreur UserThreadCreate\n");
    }
    PutString("Fin du main : \n");
    return 0;
}
