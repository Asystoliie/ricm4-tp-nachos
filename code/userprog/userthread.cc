#include "userthread.h"
#include "forkprocess.h"

void StartUserThread(int thread) {
    UserThread *t = (UserThread *) thread;
    // L'id du thread informe egalement le numéro de page du thread
    currentThread->space->InitThreadRegisters(t->func, t->arg, t->getZone());
    machine->semThreadFork->V();
    machine->Run();
}

UserThread::UserThread(const char *debugName, int f, int a, int callback) : Thread(debugName) {
    this->func = f;
    this->arg = a;
    // A la fin du thread on appel cette nouvelle fonction
    this->UpdateCallBackRegister(callback);
}

void UserThread::Fork () {
    DEBUG ('t', "Forking userThread \"%s\"\n", getName ());
    Thread::Fork (StartUserThread, (int) this);
}

void UserThread::UpdateCallBackRegister(int value) {
    this->userRegisters[31] = value;
}

int do_UserThreadCreate(int f, int arg, int callback) {
    machine->semThreadFork->P();
    UserThread* newThread = new UserThread((char*)f, f, arg, callback);
    if (newThread == NULL) { return -1; } // Erreur

    int zone = currentThread->space->GetNewZone();
    if (zone < 0) { delete newThread; return 0; }

    int thread_id = currentThread->space->GetNewThreadId(zone);
    currentThread->space->UpdateRunningThreads(1); // appel atomique

    newThread->setId(thread_id);
    newThread->setZone(zone);
    // Avant de commencer on prend le jetton, pour que tout thread qui appelle
    // userThreadJoin sur moi soit bloqué.
    currentThread->space->semJoinThreads[newThread->getZone()]->P();
    // "Fork atomique"
    newThread->Fork();
    return newThread->getId();

}

void do_UserThreadExit() {
    currentThread->space->UpdateRunningThreads(-1); // appel atomique

    // Je libere les threads en attente sur moi
    currentThread->space->semJoinThreads[currentThread->getZone()]->V();
    // Plusieurs threads peuvent attendre que je me termine.
    // Il faut donc que dans la fonction join, les threads en attente se
    // reveillent les uns les autres

    // Mise a jour de la bitmap et de la map {thread-id <-> numéro zone}
    currentThread->space->FreeBitMap(); // appel atomique
    currentThread->space->RemoveId(currentThread->getZone()); // appel atomique

    // Si je suis le thread seul/dernier thread, je termine le processus
    if(currentThread->space->Alone()) {
        do_Exit();
    }
    else {
        currentThread->Finish();
    }
}

int do_UserThreadJoin(int thread_id) {
    int zone = currentThread->space->GetZoneFromThreadId(thread_id);
    if (zone < 0)
        return zone;
    currentThread->space->semJoinThreads[zone]->P();
    // On reveille le suivant qui peut etre soit le prochain thread à qui
    // on a allouer la zone, soit un autre thread qui avait appeller join
    currentThread->space->semJoinThreads[zone]->V();
    return 0;
}

