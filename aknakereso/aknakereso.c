#include "aknakereso.h"

#include "debugmalloc.h"

void aknakereso(Megjelenites *pm, Jatek *pj){
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
