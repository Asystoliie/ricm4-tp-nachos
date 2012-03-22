#include <time.h>

#include "frameprovider.h"
#include "system.h"


FrameProvider::FrameProvider (int n) {
    this->lenght = n;
    this->bitmap = new BitMap(this->lenght);
    this->semFrameBitMap = new Semaphore("semFrameBitMap", 1);
}

FrameProvider::~FrameProvider () {
    delete bitmap;
}

void FrameProvider::ReleaseFrame(int n) {
    this->semFrameBitMap->P();
    this->bitmap->Clear(n);
    this->semFrameBitMap->V();
}

int * FrameProvider::GetEmptyFrames(int n) {
    RandomInit(0);
    this->semFrameBitMap->P();
    int * frames = NULL;
    if (n <= this->bitmap->NumClear()) {
        frames = new int[n];
        for(int i=0; i<n; i++) {
            int frame = Random()%NumPhysPages;
            // Le +1 c'est juste car des fois frame = 0...
            while(this->bitmap->Test(frame)) {
                frame = Random()%NumPhysPages;
            }
            this->bitmap->Mark(frame);
            bzero(&(machine->mainMemory[ PageSize * frame ] ), PageSize );
            frames[i] = frame;
        }
    }
    this->semFrameBitMap->V();
    return frames;
}

