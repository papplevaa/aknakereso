#include "aknakereso.h"

#include "debugmalloc.h"

int main(int argc, char *argv[]){
    Jatek jatek;
    // felhasznaloi interakcio lesz
    jatek.mag = 25;
    jatek.szel = 25;
    jatek.akna_db = 25;
    uj_jatek(&jatek);


    Megjelenites megjelenites;
    megjelenites.ablak_mag = 640;
    megjelenites.ablak_szel = 800;
    megjelenites.ikon_meret = 16;

    /** SDL */
    /* ablak letrehozasa */
    sdl_init(&megjelenites);
    /* kep betoltese */
    sdl_load_texture(&megjelenites);

    // palyakezdo koordinata
    int palya_x = megjelenites.ablak_szel/2 - jatek.szel*megjelenites.ikon_meret/2;
    int palya_y = megjelenites.ablak_mag/2 - jatek.mag*megjelenites.ikon_meret/2;

    /* konzol ujranyitasa, SDL_Init utan kell */
    #ifdef __WIN32__
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    #endif

    boxRGBA(megjelenites.renderer, 0, 0, megjelenites.ablak_szel-1, megjelenites.ablak_mag-1, 0xFF, 0xC3, 0x00, 0xFF);
    tabla_rajzol(&megjelenites, &jatek);
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
        if(vege == 0){
            if(felderites){
                felderit(&jatek, x, y);
                tabla_rajzol(&megjelenites, &jatek);
                //printf("%d\n", jatek.akna_db-jatek.zaszlo_db);
            }
            else if(jeloles){
                jelol(&jatek, x, y);
                tabla_rajzol(&megjelenites, &jatek);
                printf("%d\n", jatek.akna_db-jatek.zaszlo_db);
            }
            vege = vege_van(&jatek);
        }
        else if(vege == 1){
            boxRGBA(megjelenites.renderer, 0, 0, megjelenites.ablak_szel-1, megjelenites.ablak_mag-1, 0x90, 0xE0, 0x90, 0xFF);
            felfed(&megjelenites, &jatek);
        }
        else if(vege == -1){
            boxRGBA(megjelenites.renderer, 0, 0, megjelenites.ablak_szel-1, megjelenites.ablak_mag-1, 0xFF, 0x57, 0x33, 0xFF);
            felfed(&megjelenites, &jatek);
        }
    }

    /* felszabaditjuk a memoriat */
    SDL_DestroyTexture(megjelenites.mezok);

    /* SDL vege */
    SDL_Quit();

    /* Jateknak foglalt memoriaterulet felszabaditasa */
    free(jatek.palya[0]);
    free(jatek.palya);

    return 0;
}
