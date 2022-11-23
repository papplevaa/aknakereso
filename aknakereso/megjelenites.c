#include "aknakereso.h"

#include "debugmalloc.h"

void sdl_init(Megjelenites *pm) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Aknakereső", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pm->ablak_szel, pm->ablak_mag, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    pm->window = window;
    pm->renderer = renderer;
}

void sdl_load_texture(Megjelenites *pm){
    pm->mezok = IMG_LoadTexture(pm->renderer, "img/texture.png");
    if(pm->mezok == NULL){
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
}

void mezo_rajzol(Megjelenites *pm, int x, int y) {
    SDL_Rect src = { (pm->melyik % 4) * MERET, (pm->melyik / 4) * MERET, MERET, MERET };
    SDL_Rect dest = { x, y, MERET, MERET };
    SDL_RenderCopy(pm->renderer, pm->mezok, &src, &dest);
}

void tabla_rajzol(Megjelenites *pm, Jatek *pj){
    int palya_x = WINDOW_SZEL/2 - pj->szel*MERET/2;
    int palya_y = WINDOW_MAG/2 - pj->mag*MERET/2;

    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].lathato){
                if(pj->palya[y][x].akna)
                    pm->melyik = 12;
                else
                    pm->melyik = pj->palya[y][x].ertek;

            } else if(pj->palya[y][x].jelolt){
                pm->melyik = 10;
            } else {
                pm->melyik = 9;
            }
            mezo_rajzol(pm, x * MERET + palya_x, y * MERET + palya_y);
        }
    }
    SDL_RenderPresent(pm->renderer);
}

void felfed(Megjelenites *pm, Jatek *pj){
    int palya_x = WINDOW_SZEL/2 - pj->szel*MERET/2;
    int palya_y = WINDOW_MAG/2 - pj->mag*MERET/2;
    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].akna)
                pm->melyik = 12;
            else
                pm->melyik = pj->palya[y][x].ertek;
            mezo_rajzol(pm, x * MERET + palya_x, y * MERET + palya_y);
        }
    }
    SDL_RenderPresent(pm->renderer);
}
