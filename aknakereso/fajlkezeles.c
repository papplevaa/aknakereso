#include "debugmalloc.h"

#include "fajlkezeles.h"

/* A jatekallapot menteseert felel, igaz erteket ad, ha sikeres volt a mentes */
bool mentes(Jatek *pj){
    FILE *fp = fopen("aknakereso.txt", "w");
    if(fp == NULL)
        return false;

    fprintf(fp, "Aknakereso v1\n"
                "%dx%d\n"
                "%d\n"
                "a:%d z:%d\n"
                "allapot:%d\n\n", pj->szel, pj->mag, pj->ido, pj->akna_db, pj->zaszlo_db, pj->vege);

    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x)
            fprintf(fp, "%d%d%d%d;", pj->palya[y][x].ertek, pj->palya[y][x].akna, pj->palya[y][x].lathato, pj->palya[y][x].jelolt);
        fprintf(fp, "\n");
    }
    fclose(fp);
    return true;
}

/* A jatekallapot beolvasasaert felel egy aknakereso.txt fajlbol, igaz erteket ad, ha sikeres volt a beolvasas */
bool betoltes(Jatek *pj){
    FILE *fp = fopen("aknakereso.txt", "r");
    if(fp == NULL)
        return false;

    int verzio;
    char sor[255];
    fscanf(fp, "Aknakereso v%d\n"
               "%dx%d\n"
               "%d\n"
               "a:%d z:%d\n"
                "allapot:%d\n", &verzio, &pj->szel, &pj->mag, &pj->ido, &pj->akna_db, &pj->zaszlo_db, &pj->vege);

    bool ok;
    ok = foglal(pj);
    if(!ok)
        return false;

    for(int y = 0; y < pj->mag; ++y){
        fgets(sor, 255, fp);
        int i = 0;
        int x = 0;
        while(sor[i] != '\n'){
            pj->palya[y][x].ertek = sor[i++] - '0';
            pj->palya[y][x].akna = sor[i++] - '0';
            pj->palya[y][x].lathato = sor[i++] - '0';
            pj->palya[y][x].jelolt = sor[i++] - '0';
            i++;
            x++;
        }
    }
    fclose(fp);
    return true;
}
