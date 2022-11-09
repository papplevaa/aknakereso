#ifndef MEGJELENITES_H
#define MEGJELENITES_H

#include<SDL.h>
#include<SDL_image.h>

typedef enum Mezo{
    FUres, Egy, Ketto, Harom,
    Negy, Ot, Hat, Het,
    Nyolc, Ures, Zaszlo, PZaszlo,
    Akna, PAkna
}Mezo;

enum { MERET = 16 };

void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void mezo_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, Mezo melyik, int x, int y);
void tabla_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, Jatek *jatek);

#endif // MEGJELENITES_H
