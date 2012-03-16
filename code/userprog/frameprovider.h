#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"
#include "filesys.h"
#include "synch.h"

class FrameProvider {
    public:
        FrameProvider (int n, bool rand);
        FrameProvider (int n);
        // Initialise le FrameProvider de n cadres.
        ~FrameProvider ();
        int GetEmptyFrame ();
        // Retourne le numero d'un cadre libre et initialise la page à zéro
        void ReleaseFrame(int n);
        // Libere le cadre Nieme cadre
        int NumAvailFrame();
        // Retourne le nombre de cadres disponibles

    private:
        int lenght;
        bool random_acces;
        BitMap * bitmap;
        Semaphore * semFrameBitMap;
};

#endif // FRAMEPROVIDER_H

