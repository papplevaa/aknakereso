#include<stdio.h>

#include "jatekmenet.h"

void rajzol(Jatek *field);
void rajzol2(Jatek *field);

int main(int argc, char *argv[]){
    Jatek jatek;

    // felhasznaloi interakcio lesz
    jatek.mag = 10;
    jatek.szel = 10;
    jatek.akna_db = 25;

    uj_jatek(&jatek);

    rajzol2(&jatek);
    printf("\n");

    // TESZT GAMME JELOLESRE ES FELDERITESRE
    int x, y, o;
    do{
        scanf("%d", &o);
        switch(o){
        case 0:
            scanf("%d%d", &x, &y);
            felderit(&jatek, x-1, y-1);
            rajzol(&jatek);
            break;
        case 1:
            scanf("%d%d", &x, &y);
            jelol(&jatek, x-1, y-1);
            rajzol(&jatek);
            break;
        }
    }while(o!=-1);

    return 0;
}














void rajzol(Jatek *field){
    for(int y = 0; y < field->mag; ++y){
        for(int x = 0; x < field->szel; ++x){
            if(field->palya[y][x].lathato){
                if(field->palya[y][x].akna)
                    printf("A");
                else
                    printf("%d", field->palya[y][x].ertek);
            } else if(field->palya[y][x].jelolt){
                printf("F");
            } else {
                printf("X");
            }
        }
        printf("\n");
    }
}

void rajzol2(Jatek *field){
    for(int y = 0; y < field->mag; ++y){
        for(int x = 0; x < field->szel; ++x){
            //if(field->palya[y][x].felderitett){
                if(field->palya[y][x].akna)
                    printf("A");
                else
                    printf("%d", field->palya[y][x].ertek);
            //} else {
            //    printf("X");
            //}
        }
        printf("\n");
    }
}
