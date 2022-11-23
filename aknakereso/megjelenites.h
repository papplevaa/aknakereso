#ifndef MEGJELENITES_H
#define MEGJELENITES_H

#include<SDL.h>
#include<SDL_image.h>
#include <SDL2_gfxPrimitives.h>

/** Grafikahoz **/
// textura ikonjai
typedef enum Mezo{
    FUres, Egy, Ketto, Harom,
    Negy, Ot, Hat, Het,
    Nyolc, Ures, Zaszlo, PZaszlo,
    Akna, PAkna
}Mezo;

// megjeleniteshez szukseges struktura
typedef struct Megjelenites{
    int ikon_meret;
    int ablak_szel, ablak_mag;
    int palya_x, palya_y;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *mezok;
    Mezo melyik;
}Megjelenites;

// aknak merete
enum { MERET = 16 };
enum { WINDOW_MAG = 640, WINDOW_SZEL = 800};

/* Grafikahoz szukseges fv-ek */
void sdl_init(Megjelenites *pm);
void sdl_load_texture(Megjelenites *pm);
void mezo_rajzol(Megjelenites *pm, int x, int y);
void tabla_rajzol(Megjelenites *pm, Jatek *pj);
void felfed(Megjelenites *pm, Jatek *pj);

#endif // MEGJELENITES_H
