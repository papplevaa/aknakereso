#include "debugmalloc.h"

#include "jatekmenet.h"

/* Memoriat foglal a jatekmenethez (Jatek struct) */
bool foglal(Jatek *pj){
    /* Pointertombnek memoriafoglalas */
    pj->palya = (Cella**) malloc(pj->mag * sizeof(Cella*));
    if(pj->palya == NULL)
        return false;

    /* A pointertomb elso cimere mag * szel meretu 1D tombnek memoriafoglalas */
    pj->palya[0] = (Cella*) malloc(pj->mag * pj->szel * sizeof(Cella));
    if(pj->palya[0] == NULL)
        return false;

    /* Pointertomb inicializalasa uj "sorokhoz" merten */
    for(int y = 1; y < pj->mag; ++y)
        pj->palya[y] = pj->palya[0] + y * pj->szel;

    return true;
}

/* Felszabaditja a foglalt memoriateruletet */
void felszabadit(Jatek *pj){
    free(pj->palya[0]);
    free(pj->palya);
    pj->palya = NULL;
}

/* Lepes szabalyossagat ellenorzi */
static bool szabalyos(Jatek *pj, int x, int y){
    return !(x < 0 || x >= pj->szel || y < 0 || y >= pj->mag);
}

/* A palyat inicializalja */
void inicializal(Jatek *pj){
    /* Kezdoertekek 0-ra allitasa */
    pj->ido = 0;
    pj->zaszlo_db = 0;
    pj->vege = JATEKBAN;
    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            pj->palya[y][x].ertek = 0;
            pj->palya[y][x].akna = false;
            pj->palya[y][x].lathato = false;
            pj->palya[y][x].jelolt = false;
        }
    }

    /* Aknak elhelyezese */
    srand(time(0));
    int i = 0;
    while(i < pj->akna_db){
        int x, y;
        x = rand() % pj->szel;
        y = rand() % pj->mag;
        if(pj->palya[y][x].akna)
            continue;
        pj->palya[y][x].akna = true;
        i++;
    }

    // cellak kiertekelese (hany szomszedos akna van?)
    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            for(int i = -1; i <= 1; ++i){
                for(int j = -1; j <= 1; ++j){
                    if(szabalyos(pj, x+j, y+i)){
                        if(pj->palya[y+i][x+j].akna){
                            pj->palya[y][x].ertek++;
                        }
                    }
                }
            }
        }
    }
}

/* Automatikus felderites */
/* A felderites hajtoereje, rekurziot hasznal a palya bejarasahoz */
void felderit_seged(Jatek *pj, int x, int y){
    if(szabalyos(pj, x, y)){
        /* Rekurzio feltetele: cella erteke 0 es nem lathato -> nezzuk meg a szomszedos cellakat is */
        if(pj->palya[y][x].ertek == 0 && !pj->palya[y][x].lathato){
            /* Legyen lathato, hisz felderitunk */
            pj->palya[y][x].lathato = true;
            /* Ha jelolt volt eddig, akkor ne legyen */
            /* Ennek megfeleloen a zaszlo_db szamlalot is csokketnjuk */
            if(pj->palya[y][x].jelolt){
                pj->palya[y][x].jelolt = false;
                pj->zaszlo_db -= 1;
            }
            /* 8 szomszedos cella felderitese */
            felderit_seged(pj, x-1, y-1);
            felderit_seged(pj, x, y-1);
            felderit_seged(pj, x+1, y-1);
            felderit_seged(pj, x-1, y);
            felderit_seged(pj, x+1, y);
            felderit_seged(pj, x-1, y+1);
            felderit_seged(pj, x, y+1);
            felderit_seged(pj, x+1, y+1);
        } else {
            pj->palya[y][x].lathato = true;
            if(pj->palya[y][x].jelolt){
                pj->palya[y][x].jelolt = false;
                pj->zaszlo_db -= 1;
            }
        }
    }
}

/* Csak szabalyos es nem jelolt cellat ad oda a felderit_seged()-nek */
void felderit(Jatek *pj, int x, int y){
    if(szabalyos(pj, x, y) && !pj->palya[y][x].jelolt){
            felderit_seged(pj, x, y);
    }
}

/* Mezo megjelolese/jeloles megszuntetese */
void jelol(Jatek *pj, int x, int y){
    if(szabalyos(pj, x, y) && !pj->palya[y][x].lathato){
        pj->palya[y][x].jelolt = !pj->palya[y][x].jelolt;

        if(pj->palya[y][x].jelolt)
            pj->zaszlo_db += 1;
        else
            pj->zaszlo_db -= 1;
    }
}

/* Jatek veget ellenorzi
 * Visszateresi erteke
    - 0, ha felderitett aknat talalt, azaz vege a jateknak
    - 1, ha meg jatekban vagyunk, mert van meg felderitetlen / jeloletlen cella
    - 2, ha megnyertuk a jatekot. */
int vege_van(Jatek *pj){
    bool van_ures = false;
    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].akna && pj->palya[y][x].lathato)
                return 0;
            if((!pj->palya[y][x].akna && !pj->palya[y][x].lathato) || (pj->palya[y][x].akna && !pj->palya[y][x].jelolt))
                van_ures = true;
        }
    }
    if(van_ures)
        return 1;
    else
        return 2;
}
