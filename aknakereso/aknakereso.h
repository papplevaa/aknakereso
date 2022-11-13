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
    // Jatekhoz
    Cella **palya;
    int szel, mag;
    int akna_db, zaszlo_db;

    // Grafikahoz
    //SDL_Window *window;
    //SDL_Renderer *renderer;
    //SDL_Texture *mezok;
    //Mezo mezo;
}Jatek;

/** Grafikahoz **/
// textura ikonjai
typedef enum Mezo{
    FUres, Egy, Ketto, Harom,
    Negy, Ot, Hat, Het,
    Nyolc, Ures, Zaszlo, PZaszlo,
    Akna, PAkna
}Mezo;

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
void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void mezo_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, Mezo melyik, int x, int y);
void tabla_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, Jatek *jatek);
void felfed(SDL_Renderer *renderer, SDL_Texture *mezok, Jatek *jatek);

#endif // AKNAKERESO_H
