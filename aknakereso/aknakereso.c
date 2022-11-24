#include "menu.h"

#include "debugmalloc.h"

int main(int argc, char *argv[]){
    /* konzol ujranyitasa, SDL_Init utan kell */
    #ifdef __WIN32__
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    #endif

    /* Jatek */
    Jatek jatek;

    /* SDL */
    Megjelenites megjelenites;
    sdl_init(&megjelenites);
    sdl_load_texture(&megjelenites);

    switch(fomenu(&megjelenites)){
        case UJ_JATEK:
            SDL_RenderClear(megjelenites.renderer);
            masodlagos_menu(&megjelenites, &jatek);
            uj_jatek(&jatek);
            megjelenites.palya_x = WINDOW_SZEL/2 - jatek.szel*MERET/2;
            megjelenites.palya_y = WINDOW_MAG/2 - jatek.mag*MERET/2;
            jatekmenu(&megjelenites, &jatek);
            break;
        case BETOLTES:
//            betoltes();
//            aknakereso();
            break;
    }

    SDL_DestroyTexture(megjelenites.mezok);
    SDL_DestroyRenderer(megjelenites.renderer);
    SDL_DestroyWindow(megjelenites.window);
    SDL_Quit();

    /* Jateknak foglalt memoriaterulet felszabaditasa */
    free(jatek.palya[0]);
    free(jatek.palya);

    return 0;
}
