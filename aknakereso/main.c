#include "aknakereso.h"

#include "debugmalloc.h"

int main(int argc, char *argv[]){
    /** GAMME LETREHOZASA */
    Jatek jatek;

    // felhasznaloi interakcio lesz
    jatek.mag = 25;
    jatek.szel = 25;
    jatek.akna_db = 15;

    uj_jatek(&jatek);

    /** SDL */
    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(WINDOW_SZEL, WINDOW_MAG, &window, &renderer);

    // palyakezdo koordinata
    int palya_x = WINDOW_SZEL/2 - jatek.szel*MERET/2;
    int palya_y = WINDOW_MAG/2 - jatek.mag*MERET/2;

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

    boxRGBA(renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0xFF, 0xC3, 0x00, 0xFF);
    tabla_rajzol(renderer, mezok, &jatek);
    /* ESEMENYHUROK */
    bool quit = false;
    int vege = 0;
    int mouse_x, mouse_y;
    while(!quit){
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        bool felderites = false;
        bool jeloles = false;

        int x, y;
        // event esetek
        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                mouse_x = ev.button.x;
                mouse_y = ev.button.y;
                x = (mouse_x - palya_x) / MERET;
                y = (mouse_y - palya_y) / MERET;
                if(ev.button.button == SDL_BUTTON_LEFT){
                    felderites = true;
                }else if(ev.button.button == SDL_BUTTON_RIGHT){
                    jeloles = true;
                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }

        // eldonti hogy aktualis event mit csinaljon
        if(felderites && vege == 0){
            felderit(&jatek, x, y);
            tabla_rajzol(renderer, mezok, &jatek);
            printf("%d\n", jatek.akna_db-jatek.zaszlo_db);
            vege = vege_van(&jatek);
        }
        else if(jeloles && vege == 0){
            jelol(&jatek, x, y);
            tabla_rajzol(renderer, mezok, &jatek);
            printf("%d\n", jatek.akna_db-jatek.zaszlo_db);
            vege = vege_van(&jatek);
        }

        if(vege == 1){
            boxRGBA(renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x90, 0xE0, 0x90, 0xFF);
            felfed(renderer, mezok, &jatek);
        }
        else if(vege == -1){
            boxRGBA(renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0xFF, 0x57, 0x33, 0xFF);
            felfed(renderer, mezok, &jatek);
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
