#include "frameprovider.h"
#include "system.h"


FrameProvider::~FrameProvider () {
    delete bitmap;
}


FrameProvider::FrameProvider (int n, bool rand) {
    this->lenght = n;
    this->random_acces = rand;
    this->bitmap = new BitMap(this->lenght);
    this->semFrameBitMap = new Semaphore("semFrameBitMap", 1);
}

FrameProvider::FrameProvider (int n) {
    this->lenght = n;
    this->random_acces = false;
    this->bitmap = new BitMap(this->lenght);
    this->semFrameBitMap = new Semaphore("semFrameBitMap", 1);
}


int FrameProvider::GetEmptyFrame() {
    int frame = -1;
    this->semFrameBitMap->P();
    frame = this->bitmap->Find();
    this->semFrameBitMap->V();
    if (frame != -1)
        bzero(&(machine->mainMemory[ PageSize * frame ] ), PageSize );
    return frame;
}


void FrameProvider::ReleaseFrame(int n) {
    this->semFrameBitMap->P();
    this->bitmap->Clear(n);
    this->semFrameBitMap->V();
}

int FrameProvider::NumAvailFrame() {
    this->semFrameBitMap->P();
    int num = 0;
    for(int i = 0; i<this->lenght; i++) {
        if (!this->bitmap->Test(i))
            num++;
    }
    this->semFrameBitMap->V();
    return num;
}

