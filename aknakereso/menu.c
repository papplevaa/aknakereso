#include "menu.h"

#include "debugmalloc.h"



/* Beolvas egy szoveget a billentyuzetrol.
 * A rajzolashoz hasznalt font es a megjelenito az utolso parameterek.
 * Az elso a tomb, ahova a beolvasott szoveg kerul.
 * A masodik a maximális hossz, ami beolvasható.
 * A visszateresi erteke logikai igaz, ha sikerult a beolvasas. */
static bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
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
    return kilep;
}

static int adat_beker(Megjelenites *pm, int min, int max){
    // input_text ide ment
    char adat_str[10];
    int adat_int;

    // input_text keret es kurzor szine es hatter szine
    SDL_Color feher = {255, 255, 255}, fekete = { 0, 0, 0 };

    // input_text szovegdoboza
    int szel = WINDOW_SZEL/2;
    int mag = 40;
    int kezd_x = (WINDOW_SZEL - szel)/2;
    int kezd_y = (WINDOW_MAG - mag)/2;
    SDL_Rect text = { kezd_x, kezd_y, szel, mag };

    bool kilep = false;
    // addig olvas, amig helyes erteket nem kap
    do{
        kilep = input_text(adat_str, 10, text, fekete, feher, pm->font, pm->renderer);
        if(kilep)
            return -1;
        adat_int = atoi(adat_str);
    } while((adat_int < min) || (adat_int > max) || adat_int == 0);

    // visszaadja abeolvasott erteket
    return adat_int;
}

int fomenu(Megjelenites *pm){
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    szoveg(pm, "Új játék", 0, 0, WINDOW_SZEL-1, WINDOW_MAG/2);
    szoveg(pm, "Betöltés", 0, WINDOW_MAG/2, WINDOW_SZEL-1, WINDOW_MAG-1);
    thickLineRGBA(pm->renderer, 0, WINDOW_MAG/2, WINDOW_SZEL-1, WINDOW_MAG/2, 3, 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_RenderPresent(pm->renderer);
    SDL_RenderClear(pm->renderer);

    bool kilep = false;
    while(!kilep){
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.y <= WINDOW_MAG/2)
                    return UJ_JATEK;
                else
                    return BETOLTES;
                break;
            case SDL_QUIT:
                kilep = true;
                break;
        }
    }
    return -1;
}

bool uj_jatek_menu(Megjelenites *pm, Jatek *pj){
    int min, max, adat;
    char str[100];

    min = 5;
    max = WINDOW_SZEL / MERET;
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    sprintf(str, "Pálya szélessége (%d..%d):", min, max);
    szoveg(pm, str, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-2*FEJLEC);
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->szel = adat;
    SDL_RenderClear(pm->renderer);

    min = 5;
    max = (WINDOW_MAG-32) / MERET;
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    sprintf(str, "Pálya magassága (%d..%d):", min, max);
    szoveg(pm, str, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-2*FEJLEC);
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->mag = adat;
    SDL_RenderClear(pm->renderer);

    min = 1;
    max = pj->szel * pj->mag * AKNA_ARANY/100;
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    sprintf(str, "Aknák száma (%d..%d):", min, max);
    szoveg(pm, str, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-2*FEJLEC);
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->akna_db = adat;
    SDL_RenderClear(pm->renderer);

    foglal(pj);
    inicializal(pj);
    return true;
}

Uint32 idozito(Uint32 ms, void *param){
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

void jatekmenu(Megjelenites *pm, Jatek *pj){
    //
    bool kilep = false;
    // palya bal felso pontja
    int palya_x = WINDOW_SZEL/2 - (pj->szel*MERET)/2;
    int palya_y = FEJLEC + (WINDOW_MAG-FEJLEC)/2 - pj->mag*MERET/2;
    //
    int x, y;
    //
    SDL_TimerID id = SDL_AddTimer(1000, idozito, NULL);

    // elso kirajzolas
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    fejlec(pm, pj);
    tabla_rajzol(pm, pj);
    pj->ido++;

    // esemenyhurok
    while(!kilep){
        bool felderites = false;
        bool jeloles = false;
        SDL_Event ev;
        SDL_WaitEvent(&ev);

        switch(ev.type){
            case SDL_USEREVENT:
                fejlec(pm, pj);
                pj->ido++;
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = (ev.button.x - palya_x) / MERET;
                y = (ev.button.y - palya_y) / MERET;
                if(ev.button.button == SDL_BUTTON_LEFT)
                    felderites = true;
                else if(ev.button.button == SDL_BUTTON_RIGHT)
                    jeloles = true;
                break;

            case SDL_QUIT:
                kilep = true;
                break;
        }

        if(pj->vege == JATEKBAN){
            if(felderites)
                felderit(pj, x, y);
            else if(jeloles)
                jelol(pj, x, y);
            pj->vege = vege_van(pj);
            fejlec(pm, pj);
            tabla_rajzol(pm, pj);
        } else {
            SDL_RemoveTimer(id);
            fejlec(pm, pj);
            felfed(pm, pj);
        }

        mentes(pj);
    }
}
