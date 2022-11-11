#include "aknakereso.h"

#include "debugmalloc.h"

int main(int argc, char *argv[]){
    /** GAMME LETREHOZASA */
    Jatek jatek;

    // felhasznaloi interakcio lesz
    jatek.mag = 25;
    jatek.szel = 25;
    jatek.akna_db = 80;

    uj_jatek(&jatek);

    /** SDL */
    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(jatek.mag*MERET, jatek.szel*MERET, &window, &renderer);

    /* konzol ujranyitasa, SDL_Init utan kell */
    #ifdef __WIN32__
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    #endif

    /* kep betoltese */
    SDL_Texture *mezok = IMG_LoadTexture(renderer, "img/texture.png");
    //SDL_Surface *icon = IMG_Load("img/icon.jpg");
    //SDL_SetWindowIcon(window, icon);
    if(mezok == NULL){
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    tabla_rajzol(renderer, mezok, &jatek);

    /* ESEMENYHUROK */
    bool quit = false;
    int mouse_x, mouse_y;
    while(!quit){
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        bool felderites = false;
        bool jeloles = false;

        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.button == SDL_BUTTON_LEFT){
                    mouse_x = ev.button.x;
                    mouse_y = ev.button.y;
                    felderites = true;
                }else if(ev.button.button == SDL_BUTTON_RIGHT){
                    mouse_x = ev.button.x;
                    mouse_y = ev.button.y;
                    jeloles = true;
                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }

        int x = mouse_x / MERET;
        int y = mouse_y / MERET;

        if(felderites){
            felderit(&jatek, x, y);
            tabla_rajzol(renderer, mezok, &jatek);
        }else if(jeloles){
            jelol(&jatek, x, y);
            tabla_rajzol(renderer, mezok, &jatek);
        }

    }

    /* felszabaditjuk a memoriat */
    SDL_DestroyTexture(mezok);


    /* SDL vege */
    SDL_Quit();

    /* Jateknak foglalt memoriaterulet felszabaditasa */
    free(jatek.palya[0]);
    free(jatek.palya);

    return 0;
}
