#include "fajlkezeles.h"

bool mentes(Jatek *pj){
    FILE *fp = fopen("save/jatek.txt", "w");
    if(fp == NULL)
        return false;

    fprintf(fp, "Aknakereso v1\n"
                "%dx%d\n"
                "%d"
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

bool betoltes(Jatek *pj){
    FILE *fp = fopen("save/jatek.txt", "r");
    if(fp == NULL)
        return false;

    int verzio;
    char sor[255];
    fscanf(fp, "Aknakereso v%d\n"
               "%dx%d\n"
               "%d\n"
               "a:%d z:%d\n"
                "allapot:%d\n", &verzio, &pj->szel, &pj->mag, &pj->ido, &pj->akna_db, &pj->zaszlo_db, &pj->vege);

    foglal(pj);

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
