#include "aknakereso.h"

#include "debugmalloc.h"

void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Aknakereső", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
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

    *pwindow = window;
    *prenderer = renderer;
}

void mezo_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, Mezo melyik, int x, int y) {
    SDL_Rect src = { (melyik % 4) * MERET, (melyik / 4) * MERET, MERET, MERET };
    SDL_Rect dest = { x, y, MERET, MERET };
    SDL_RenderCopy(renderer, mezok, &src, &dest);
}

void tabla_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, Jatek *jatek){
    int palya_x = WINDOW_SZEL/2 - jatek->szel*MERET/2;
    int palya_y = WINDOW_MAG/2 - jatek->mag*MERET/2;
    for(int y = 0; y < jatek->mag; ++y){
        for(int x = 0; x < jatek->szel; ++x){
            Mezo melyik;
            if(jatek->palya[y][x].lathato){
                if(jatek->palya[y][x].akna)
                    melyik = 12;
                else
                    melyik = jatek->palya[y][x].ertek;

            } else if(jatek->palya[y][x].jelolt){
                melyik = 10;
            } else {
                melyik = 9;
            }
            mezo_rajzol(renderer, mezok, melyik, x * MERET + palya_x, y * MERET + palya_y);
        }
    }
    SDL_RenderPresent(renderer);
}

void felfed(SDL_Renderer *renderer, SDL_Texture *mezok, Jatek *jatek){
    int palya_x = WINDOW_SZEL/2 - jatek->szel*MERET/2;
    int palya_y = WINDOW_MAG/2 - jatek->mag*MERET/2;
    for(int y = 0; y < jatek->mag; ++y){
        for(int x = 0; x < jatek->szel; ++x){
            Mezo melyik;
            if(jatek->palya[y][x].akna)
                melyik = 12;
            else
                melyik = jatek->palya[y][x].ertek;
            mezo_rajzol(renderer, mezok, melyik, x * MERET + palya_x, y * MERET + palya_y);
        }
    }
    SDL_RenderPresent(renderer);
}
