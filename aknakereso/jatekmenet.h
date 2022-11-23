#ifndef JATEKMENET_H
#define JATEKMENET_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

/** Szukseges adatstrukturak **/
typedef struct Cella{
    int ertek;
    bool akna, lathato, jelolt;
}Cella;

typedef struct Jatek{
    Cella **palya;
    int szel, mag;
    int akna_db, zaszlo_db;
}Jatek;

enum { VESZTETT = -1, JATEKBAN = 0, NYERT = 1 };

/** Szukseges adatstrukturak **/
typedef struct Cella{
    int ertek;
    bool akna, lathato, jelolt;
}Cella;

typedef struct Jatek{
    Cella **palya;
    int szel, mag;
    int akna_db, zaszlo_db;
}Jatek;

enum { VESZTETT = -1, JATEKBAN = 0, NYERT = 1 };

/* Jatekmenethez szukseges fv-ek */
void uj_jatek(Jatek *pj);
void felderit(Jatek *pj, int x, int y);
void felderit_seged(Jatek *pj, int x, int y);
void jelol(Jatek *pj, int x, int y);
int vege_van(Jatek *pj);

#endif // JATEKMENET_H
