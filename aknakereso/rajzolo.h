#ifndef RAJZOLO_H
#define RAJZOLO_H

#include<SDL.h>
#include<SDL_image.h>
#include<SDL2_gfxPrimitives.h>
#include<SDL_ttf.h>

#include "jatekmenet.h"

/* Ablak, ikonmeret es fejlec magassag specifikacioi */
enum { WINDOW_MAG = 640, WINDOW_SZEL = 800, MERET = 16, FEJLEC = 64};

/* Textura ertelmezesehez */
typedef enum Mezo{
    FUres, Egy, Ketto, Harom,
    Negy, Ot, Hat, Het,
    Nyolc, Ures, Zaszlo, PZaszlo,
    Akna, PAkna
}Mezo;

/* Megjeleniteshez szukseges adatszerkezet */
typedef struct Megjelenites{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Texture *mezok;
}Megjelenites;

void sdl_init(Megjelenites *pm);
void sdl_close(Megjelenites *pm);
void szoveg(Megjelenites* pm, char *szoveg, int x1, int y1, int x2, int y2);
void tabla_rajzol(Megjelenites *pm, Jatek *pj);
void felfed(Megjelenites *pm, Jatek *pj);
void fejlec(Megjelenites *pm, Jatek *pj);

#endif // RAJZOLO_H
