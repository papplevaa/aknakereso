#include "menu.h"

#include "debugmalloc.h"

int main(int argc, char *argv[]){
//    /* konzol ujranyitasa, SDL_Init utan kell */
//    #ifdef __WIN32__
//        freopen("CON", "w", stdout);
//        freopen("CON", "w", stderr);
//    #endif

    Jatek jatek;
    Megjelenites megjelenites;
    sdl_init(&megjelenites);

    bool ok;
    switch(fomenu(&megjelenites)){
        case UJ_JATEK:
            ok = uj_jatek_menu(&megjelenites, &jatek);
            if(!ok)
                break;
            jatekmenu(&megjelenites, &jatek);
            felszabadit(&jatek);
            break;
        case BETOLTES:
            ok = betoltes(&jatek);
            if(!ok)
                break;
            jatekmenu(&megjelenites, &jatek);
            felszabadit(&jatek);
            break;
    }

    sdl_close(&megjelenites);

    return 0;
}
