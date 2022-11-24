#include "aknakereso.h"

#include "debugmalloc.h"

int main(int argc, char *argv[]){
    /* konzol ujranyitasa, SDL_Init utan kell */
    #ifdef __WIN32__
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    #endif

    /* Jatek */
    Jatek jatek;
//    jatek. mag = 25;
//    jatek.szel = 25;
//    jatek.akna_db = 25;
//    uj_jatek(&jatek);

    /* SDL */
    Megjelenites megjelenites;
    sdl_init(&megjelenites);
//    megjelenites.palya_x = megjelenites.ablak_szel/2 - jatek.szel*megjelenites.ikon_meret/2;
//    megjelenites.palya_y = megjelenites.ablak_mag/2 - jatek.mag*megjelenites.ikon_meret/2;
    sdl_load_texture(&megjelenites);

    switch(fomenu(&megjelenites)){
        case UJ_JATEK:
            SDL_RenderClear(megjelenites.renderer);
            masodlagos_menu(&megjelenites, &jatek);
            uj_jatek(&jatek);
            megjelenites.palya_x = megjelenites.ablak_szel/2 - jatek.szel*megjelenites.ikon_meret/2;
            megjelenites.palya_y = megjelenites.ablak_mag/2 - jatek.mag*megjelenites.ikon_meret/2;
            aknakereso(&megjelenites, &jatek);
            break;
        case BETOLTES:
//            betoltes();
//            aknakereso();
            break;
    }


//    aknakereso(&megjelenites, &jatek);

    SDL_DestroyTexture(megjelenites.mezok);
    SDL_DestroyRenderer(megjelenites.renderer);
    SDL_DestroyWindow(megjelenites.window);
    SDL_Quit();

    /* Jateknak foglalt memoriaterulet felszabaditasa */
    free(jatek.palya[0]);
    free(jatek.palya);

    return 0;
}
