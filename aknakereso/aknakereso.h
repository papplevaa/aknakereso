#ifndef AKNAKERESO_H
#define AKNAKERESO_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#include<SDL.h>
#include<SDL_image.h>
#include <SDL2_gfxPrimitives.h>

/** Szukseges adatstrukturak **/
typedef struct Cella{
    int ertek;
    bool akna, lathato, jelolt;
}Cella;

typedef struct Jatek{
    Cella **palya;
    int szel, mag;
    int akna_db, zaszlo_db;
}Jatek;

enum { VESZTETT = -1, JATEKBAN = 0, NYERT = 1 };

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

/* Jatekmenethez szukseges fv-ek */
void uj_jatek(Jatek *pj);
void felderit(Jatek *pj, int x, int y);
void felderit_seged(Jatek *pj, int x, int y);
void jelol(Jatek *pj, int x, int y);
int vege_van(Jatek *pj);

/* Grafikahoz szukseges fv-ek */
void sdl_init(Megjelenites *pm);
void sdl_load_texture(Megjelenites *pm);
void mezo_rajzol(Megjelenites *pm, int x, int y);
void tabla_rajzol(Megjelenites *pm, Jatek *pj);
void felfed(Megjelenites *pm, Jatek *pj);

#endif // AKNAKERESO_H
