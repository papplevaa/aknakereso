#ifndef JATEKMENET_H
#define JATEKMENET_H

#include<stdbool.h>

/** A palya egy cellaja */
typedef struct Cella{
    int ertek;
    bool akna;
    bool lathato;
    bool jelolt;
}Cella;

/** A jatekmenetrol minden informacio */
typedef struct Jatek{
    Cella **palya;
    int szel, mag, akna_db;
}Jatek;

/** Jatekhoz szukseges fuggvenyek */
void uj_jatek(Jatek *pj);
void felderit(Jatek *pj, int x, int y);
void jelol(Jatek *pj, int x, int y);

#endif // JATEKMENET_H
