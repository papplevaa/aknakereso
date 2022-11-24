#include"rajzolo.h"

#include "debugmalloc.h"

void mezo_rajzol(Megjelenites *pm, Mezo melyik, int x, int y) {
    SDL_Rect forras = { (melyik % 4) * MERET, (melyik / 4) * MERET, MERET, MERET };
    SDL_Rect celterulet = { x, y, MERET, MERET };
    SDL_RenderCopy(pm->renderer, pm->mezok, &forras, &celterulet);
}

void tabla_rajzol(Megjelenites *pm, Jatek *pj){
    Mezo melyik;
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

            int hova_x = pm->palya_x + x * MERET;
            int hova_y = pm->palya_y + y * MERET;
            mezo_rajzol(pm, melyik, hova_x, hova_y);
        }
    }
    SDL_RenderPresent(pm->renderer);
}

void felfed(Megjelenites *pm, Jatek *pj){
    Mezo melyik;
    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].akna)
                melyik = Akna;
            else
                melyik = pj->palya[y][x].ertek;

            int hova_x = pm->palya_x + x * MERET;
            int hova_y = pm->palya_y + y * MERET;
            mezo_rajzol(pm, melyik, hova_x, hova_y);
        }
    }
    SDL_RenderPresent(pm->renderer);
}

/* Beolvas egy szoveget a billentyuzetrol.
 * A rajzolashoz hasznalt font es a megjelenito az utolso parameterek.
 * Az elso a tomb, ahova a beolvasott szoveg kerul.
 * A masodik a maximális hossz, ami beolvasható.
 * A visszateresi erteke logikai igaz, ha sikerult a beolvasas. */
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;

            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz) {
                    strcat(dest, event.text.text);
                }

                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;

            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;

            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}

int adat_beker(Megjelenites *pm, int min, int max){
    char adat_str[10];
    int adat_int;

    SDL_Color feher = {255, 255, 255}, fekete = { 0, 0, 0 };

    int szel = WINDOW_SZEL/2;
    int mag = 40;
    int kezd_x = (WINDOW_SZEL - szel)/2;
    int kezd_y = (WINDOW_MAG - mag)/2;

    SDL_Rect text = { kezd_x, kezd_y, szel, mag };
    do{
        input_text(adat_str, 10, text, fekete, feher, pm->font, pm->renderer);
        adat_int = atoi(adat_str);
    } while(adat_int < min || adat_int > max);
    return adat_int;
}
