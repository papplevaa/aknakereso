#include"rajzolo.h"

#include "debugmalloc.h"

void hatter(Megjelenites *pm){
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    SDL_RenderPresent(pm->renderer);
}

void vonal(Megjelenites *pm, int h){
    thickLineRGBA(pm->renderer, 0, h, WINDOW_SZEL-1, h, 3, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderPresent(pm->renderer);
}

void szoveg(Megjelenites* pm, char *szoveg, int y, int h){
    SDL_Color feher = { 255, 255, 255 };
    SDL_Surface *felirat = TTF_RenderUTF8_Blended(pm->font, szoveg, feher);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(pm->renderer, felirat);

    SDL_Rect hova;
    hova.w = felirat->w;
    hova.h = felirat->h;
    hova.x = (WINDOW_SZEL - felirat->w)/2;
    hova.y = y + (h - felirat->h)/2;

    SDL_RenderCopy(pm->renderer, felirat_t, NULL, &hova);
    SDL_RenderPresent(pm->renderer);
    SDL_DestroyTexture(felirat_t);
    SDL_FreeSurface(felirat);
}

void mezo_rajzol(Megjelenites *pm, Mezo melyik, int x, int y) {
    SDL_Rect forras = { (melyik % 4) * MERET, (melyik / 4) * MERET, MERET, MERET };
    SDL_Rect celterulet = { x, y, MERET, MERET };
    SDL_RenderCopy(pm->renderer, pm->mezok, &forras, &celterulet);
}

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
