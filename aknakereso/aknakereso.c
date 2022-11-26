#include "menu.h"

#include "debugmalloc.h"

int main(int argc, char *argv[]){
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
