#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"
#include "filesys.h"
#include "synch.h"

class FrameProvider {
    public:
        FrameProvider (int n);
        // Initialise le FrameProvider de n cadres.
        ~FrameProvider ();

        // Retourne un tableau de n frames libres, ou NULL
        // Se manipule en section critique
        int * GetEmptyFrames(int n);
        // Libere le cadre Nieme cadre, a protéger par un mutex
        void ReleaseFrame(int n);

    private:
        int lenght;
        BitMap * bitmap;
        Semaphore * semFrameBitMap;
};

#endif // FRAMEPROVIDER_H

