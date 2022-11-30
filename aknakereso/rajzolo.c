#include "debugmalloc.h"

#include"rajzolo.h"

/* SDL inicializalasert felel */
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
    TTF_Font *font = TTF_OpenFont("font/Monocraft.ttf", 32);
    if(font == NULL){
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Texture *mezok = IMG_LoadTexture(renderer, "img/texture.png");
    if(mezok == NULL){
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    pm->window = window;
    pm->renderer = renderer;
    pm->font = font;
    pm->mezok = mezok;
}

/* SDL bezarasaert felel */
void sdl_close(Megjelenites *pm){
    TTF_CloseFont(pm->font);
    pm->font = NULL;

    SDL_DestroyRenderer(pm->renderer);
    pm->renderer = NULL;

    SDL_DestroyWindow(pm->window);
    pm->window = NULL;

    SDL_DestroyTexture(pm->mezok);
    pm->mezok = NULL;

    SDL_Quit();
}

/* Parameterkent kapott szoveget masol a megjelenitobe.
 * Az utolso negy parameter ket pontot ad meg, melyek egy teglalap bal felso es jobb also pontjai.
 * A szoveget ezen teglalap kozpere irjuk ki. */
void szoveg(Megjelenites* pm, char *szoveg, int x1, int y1, int x2, int y2){
    SDL_Color feher = { 255, 255, 255 };
    SDL_Surface *felirat = TTF_RenderUTF8_Blended(pm->font, szoveg, feher);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(pm->renderer, felirat);

    SDL_Rect hova;
    hova.w = felirat->w;
    hova.h = felirat->h;
    hova.x = x1 + (x2-x1 - felirat->w)/2;
    hova.y = y1 + (y2-y1 - felirat->h)/2;

    SDL_RenderCopy(pm->renderer, felirat_t, NULL, &hova);

    SDL_DestroyTexture(felirat_t);
    SDL_FreeSurface(felirat);
}

/* Egz mezot masol a megjelenitobe */
static void mezo_rajzol(Megjelenites *pm, Mezo melyik, int x, int y) {
    SDL_Rect forras = { (melyik % 4) * MERET, (melyik / 4) * MERET, MERET, MERET };
    SDL_Rect celterulet = { x, y, MERET, MERET };
    SDL_RenderCopy(pm->renderer, pm->mezok, &forras, &celterulet);
}

/* Az egesz jatektablat kirajzolja */
void tabla_rajzol(Megjelenites *pm, Jatek *pj){
    Mezo melyik;
    int palya_x = WINDOW_SZEL/2 - (pj->szel*MERET)/2;
    int palya_y = FEJLEC + (WINDOW_MAG-FEJLEC)/2 - pj->mag*MERET/2;

    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].lathato){
                if(pj->palya[y][x].akna)
                    melyik = Akna;
                else
                    melyik = pj->palya[y][x].ertek;
            } else if(pj->palya[y][x].jelolt){
                melyik = Zaszlo;
            } else {
                melyik = Ures;
            }

            int hova_x = palya_x + x * MERET;
            int hova_y = palya_y + y * MERET;
            mezo_rajzol(pm, melyik, hova_x, hova_y);
        }
    }
    SDL_RenderPresent(pm->renderer);
}

/* A valos jatektablat rajzolja ki, csak a jatek vegeztevel hivjuk meg */
void felfed(Megjelenites *pm, Jatek *pj){
    Mezo melyik;
    int palya_x = WINDOW_SZEL/2 - (pj->szel*MERET)/2;
    int palya_y = FEJLEC + (WINDOW_MAG-FEJLEC)/2 - pj->mag*MERET/2;

    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].akna)
                melyik = Akna;
            else
                melyik = pj->palya[y][x].ertek;

            int hova_x = palya_x + x * MERET;
            int hova_y = palya_y + y * MERET;
            mezo_rajzol(pm, melyik, hova_x, hova_y);
        }
    }
    SDL_RenderPresent(pm->renderer);
}

/* A fejlecet rajzolja ki: jatek allapota, jatekido, aknak es zaszlok szama */
void fejlec(Megjelenites *pm, Jatek *pj){
    char ido_str[20];
    sprintf(ido_str, "%ds", pj->ido);
    char zaszlo_str[20];
    sprintf(zaszlo_str, "A: %d Z: %d", pj->akna_db, pj->zaszlo_db);
    char allapot_str[20] = " ";
    if(pj->vege == NYERT)
        sprintf(allapot_str, "Nyertél!");
    else if(pj->vege == VESZTETT)
        sprintf(allapot_str, "Vesztettél!");

    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, FEJLEC-1, 0x6A, 0x73, 0x7B, 0xFF);
    szoveg(pm, allapot_str, 0, 0, WINDOW_SZEL/3, FEJLEC-1);
    szoveg(pm, ido_str, WINDOW_SZEL/3, 0, 2*WINDOW_SZEL/3, FEJLEC-1);
    szoveg(pm, zaszlo_str, 2*WINDOW_SZEL/3, 0, WINDOW_SZEL-1, FEJLEC-1);
}
