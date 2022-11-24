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
    int palya_x, palya_y;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *mezok;
    TTF_Font *font;
}Megjelenites;

enum { WINDOW_MAG = 640, WINDOW_SZEL = 800, MERET = 16 };

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);
void mezo_rajzol(Megjelenites *pm, Mezo melyik, int x, int y);
void tabla_rajzol(Megjelenites *pm, Jatek *pj);
void felfed(Megjelenites *pm, Jatek *pj);

#endif // RAJZOLO_H
