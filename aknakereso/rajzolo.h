#ifndef RAJZOLO_H
#define RAJZOLO_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL2_gfxPrimitives.h>
#include<SDL_ttf.h>

#include "jatekmenet.h"

typedef enum Mezo{
    FUres, Egy, Ketto, Harom,
    Negy, Ot, Hat, Het,
    Nyolc, Ures, Zaszlo, PZaszlo,
    Akna, PAkna
}Mezo;

typedef struct Megjelenites{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *mezok;
}Megjelenites;

enum { WINDOW_MAG = 640, WINDOW_SZEL = 800, MERET = 16 };

void hatter(Megjelenites *pm);
void vonal(Megjelenites *pm);
void szoveg(Megjelenites *pm, char *szoveg, int x, int y);
void mezo_rajzol(Megjelenites *pm, Mezo melyik, int x, int y);
void tabla_rajzol(Megjelenites *pm, Jatek *pj);
void felfed(Megjelenites *pm, Jatek *pj);

#endif // RAJZOLO_H
