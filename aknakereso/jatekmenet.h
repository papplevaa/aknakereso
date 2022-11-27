#ifndef JATEKMENET_H
#define JATEKMENET_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

/* Jatekallas */
typedef enum jatekallas{ VESZTETT, JATEKBAN, NYERT }jatekallas;
enum {AKNA_ARANY = 10};

/* Egy cella adatait tartalmazo adatszerkezet*/
typedef struct Cella{
    int ertek;
    bool akna, lathato, jelolt;
}Cella;

/* Egy jatek adatait tartalmazo adatszerkezet
 - Ketdimenzios cellatomb
 - Palya szelessege, magassaga (sorok es oszlopok szama)
 - Aknak es elhelyezett zaszlok szama
 - Jatekido
 - Vege van-e a jatekmenetnek (0, ha vesztett, 1, ha meg jatekban van, 2, ha nyert) */
typedef struct Jatek{
    Cella **palya;
    int szel, mag;
    int akna_db, zaszlo_db;
    int ido;
    int vege;
}Jatek;

void foglal(Jatek *pj);
void inicializal(Jatek *pj);
void felszabadit(Jatek *pj);
void felderit_seged(Jatek *pj, int x, int y);
void felderit(Jatek *pj, int x, int y);
void jelol(Jatek *pj, int x, int y);
int vege_van(Jatek *pj);

#endif // JATEKMENET_H
