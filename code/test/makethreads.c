//#include "system.h"
#include "syscall.h"

void print(int i) {
    PutString("Thread 1 : ");PutInt(i);PutChar('\n');
    UserThreadExit();
}

void print2(int i) {
    PutString("Thread 2 : ");PutInt(i);PutChar('\n');
    UserThreadExit();
}


int main() {
    PutString("Test de thread : \n");
    UserThreadCreate(print, (void *)1);
    UserThreadCreate(print2, (void *)3);

    int i = 0;
    for(i=0;i<1000000;i++){}
    return 0;
    PutString("Fin\n");
}

