#include "menu.h"

#include "debugmalloc.h"

void sdl_init(Megjelenites *pm) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Aknakereső", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SZEL, WINDOW_MAG, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("font/LiberationSerif-Regular.ttf", 32);
    if(font == NULL){
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    pm->window = window;
    pm->renderer = renderer;
    pm->font = font;
}

void sdl_load_texture(Megjelenites *pm){
    pm->mezok = IMG_LoadTexture(pm->renderer, "img/texture.png");
    if(pm->mezok == NULL){
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
}

jatekmod fomenu(Megjelenites *pm){
    // fomenu krajzolasa
    /// free elni kell ezeket?????
    SDL_Surface *image = IMG_Load("img/fomenu.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(pm->renderer, image);
    SDL_RenderCopy(pm->renderer, texture, NULL, NULL);
    SDL_RenderPresent(pm->renderer);

    bool quit = false;
    while(!quit){
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.y <= WINDOW_MAG / 2)
                    return UJ_JATEK;
                else
                    return BETOLTES;
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
    }
    // ExitSDL jon a helyere
    exit(1);
}

void masodlagos_menu(Megjelenites *pm, Jatek *pj){
    int min, max;
    min = 5;
    max = WINDOW_SZEL / MERET;
    pj->szel = adat_beker(pm, min, max);
    min = 5;
    max = WINDOW_MAG / MERET;
    pj->mag = adat_beker(pm, min, max);
    min = 1;
    max = pj->szel * pj->mag * 70/100;
    pj->akna_db = adat_beker(pm, min, max);
    SDL_RenderClear(pm->renderer);
}

void jatekmenu(Megjelenites *pm, Jatek *pj){
    // kirajzolja az ures tablat
    tabla_rajzol(pm, pj);

    // esemenyhurok
    bool quit = false;
    int x, y;
    while(!quit){
        bool felderites = false;
        bool jeloles = false;
        SDL_Event ev;
        SDL_WaitEvent(&ev);

        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                x = (ev.button.x - pm->palya_x) / MERET;
                y = (ev.button.y - pm->palya_y) / MERET;
                if(ev.button.button == SDL_BUTTON_LEFT)
                    felderites = true;
                else if(ev.button.button == SDL_BUTTON_RIGHT)
                    jeloles = true;
                break;

            case SDL_QUIT:
                quit = true;
                break;
        }

        if(pj->vege == JATEKBAN){
            if(felderites)
                felderit(pj, x, y);
            else if(jeloles)
                jelol(pj, x, y);
            pj->vege = vege_van(pj);
            (pj->vege == JATEKBAN) ? tabla_rajzol(pm, pj) : felfed(pm, pj);
        }
    }
}
