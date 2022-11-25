#ifndef JATEKMENET_H
#define JATEKMENET_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

typedef enum jatekallas{ VESZTETT, JATEKBAN, NYERT }jatekallas;

typedef struct Cella{
    int ertek;
    bool akna, lathato, jelolt;
}Cella;

typedef struct Jatek{
    Cella **palya;
    int szel, mag;
    int akna_db, zaszlo_db;
    int ido;
    jatekallas vege;
}Jatek;

void foglal(Jatek *pj);
void inicializal(Jatek *pj);
void felszabadit(Jatek *pj);
void felderit(Jatek *pj, int x, int y);
void felderit_seged(Jatek *pj, int x, int y);
void jelol(Jatek *pj, int x, int y);
jatekallas vege_van(Jatek *pj);

#endif // JATEKMENET_H
