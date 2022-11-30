#include "debugmalloc.h"

#include "menu.h"

/* Beolvas egy szoveget a billentyuzetrol.
 * A rajzolashoz hasznalt struktura az utolso parametere.
 * Az elso a tomb, ahova a beolvasott szoveg kerul.
 * A masodik a maximális hossz, ami beolvasható.
 * A visszateresi erteke logikai igaz, ha a felhasznalo ki szeretne lepni.
 * Az input_text fuggveny alapja: https://infoc.eet.bme.hu/sdl/#7 */
static bool input_text(char *dest, size_t hossz, Megjelenites *pm){
    /* A szovegdoboz es a szoveg, kurzor szine */
    SDL_Color hatter = { 0, 0, 0 };
    SDL_Color szoveg = {255, 255, 255};

    /* A szovegdoboz helye */
    int szel = WINDOW_SZEL/2;
    int mag = 40;
    int kezd_x = (WINDOW_SZEL - szel)/2;
    int kezd_y = (WINDOW_MAG - mag)/2;
    SDL_Rect teglalap = { kezd_x, kezd_y, szel, mag };

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
        boxRGBA(pm->renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(pm->renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(pm->font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(pm->renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(pm->renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(pm->renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(pm->renderer);

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

/* Az input_text felhasznalasaval addig ker be adatokat, amig helyeset nem kap
 * Visszateresi erteke az adat, illetve -1, ha ki szeretnenk lepni a menubol */
static int adat_beker(Megjelenites *pm, int min, int max){
    /* Ide ment az input_text fuggveny */
    char adat_str[10];
    int adat_int;

    /* Addig olvas, amig helyes erteket nem kapunk vagy a felhasznalo ki szeretne lepni */
    bool kilep = false;
    do{
        kilep = input_text(adat_str, 10, pm);
        if(kilep)
            return -1;
        adat_int = atoi(adat_str);
    } while((adat_int < min) || (adat_int > max) || adat_int == 0);

    // visszaadja a beolvasott erteket
    return adat_int;
}

/* A fomenu kirajzolasaert es menupont valasztasaert felel
 * Visszateresi erteke
   - 0, ha uj jatekot szeretnenk jatszani
   - 1, ha egy regit szeretnenk betolteni
   - -1, ha ki szeretnenk lepni */
int fomenu(Megjelenites *pm){
    /* Hatter, menupontok es elvalaszto vonal kirajzolasa */
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    szoveg(pm, "Új játék", 0, 0, WINDOW_SZEL-1, WINDOW_MAG/2);
    szoveg(pm, "Betöltés", 0, WINDOW_MAG/2, WINDOW_SZEL-1, WINDOW_MAG-1);
    thickLineRGBA(pm->renderer, 0, WINDOW_MAG/2, WINDOW_SZEL-1, WINDOW_MAG/2, 3, 0xFF, 0xFF, 0xFF, 0xFF);
    /* Megjelenites, majd megjelenito uritese */
    SDL_RenderPresent(pm->renderer);
    SDL_RenderClear(pm->renderer);

    /* Esemenyhurok a menupont valasztasara (UJ JATEK = 0, BETOLTES = 1, KILEPES = -1)*/
    bool kilep = false;
    while(!kilep){
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.button == SDL_BUTTON_LEFT){
                    if(ev.button.y <= WINDOW_MAG/2)
                        return 0;
                    else
                        return 1;
                }
                break;
            case SDL_QUIT:
                kilep = true;
                break;
        }
    }
    return -1;
}

/* Uj jatek menupont valasztasa eseten kirajzolja az adatbekeresre felhasznalt menuket
 * Parameterei a megjeleniteshez szukseges struktura es a beolvasott adatokhoz szukseges jatek struktura
 * Visszateresi erteke logikai igaz, ha ez sikeresen megtortent, illetve, hamis, ha a felhasznalo ki szeretne lepni */
bool uj_jatek_menu(Megjelenites *pm, Jatek *pj){
    /* Ertekadas intervalluma es az adat valtozoja */
    int min, max, adat;
    /* Cim kiiratasahoz szukseges */
    char str[100];

    /* --- PALYA SZELESSEGENEK BEKERESE --- */
    min = 5;
    max = WINDOW_SZEL / MERET;
    /* Hatterszin es cim kirajzolasa */
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    sprintf(str, "Pálya szélessége (%d..%d):", min, max);
    szoveg(pm, str, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-2*FEJLEC);
    /* Adat ellenorzese */
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->szel = adat;
    /* Megjelenito uritese */
    SDL_RenderClear(pm->renderer);

    /* --- PALYA MAGASSAGANAK BEKERESE --- */
    min = 5;
    max = (WINDOW_MAG-FEJLEC) / MERET;
    /* Hatterszin es cim kirajzolasa */
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    sprintf(str, "Pálya magassága (%d..%d):", min, max);
    szoveg(pm, str, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-2*FEJLEC);
    /* Adat ellenorzese */
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->mag = adat;
    /* Megjelenito uritese */
    SDL_RenderClear(pm->renderer);

    /* --- AKNAK SZAMANAK BEKERESE --- */
    min = 1;
    max = pj->szel * pj->mag * AKNA_ARANY/100;
    /* Hatterszin es cim kirajzolasa */
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    sprintf(str, "Aknák száma (%d..%d):", min, max);
    szoveg(pm, str, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-2*FEJLEC);
    /* Adat ellenorzese */
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->akna_db = adat;
    /* Megjelenito uritese */
    SDL_RenderClear(pm->renderer);

    /* Bekert adatok alapjan memoriat foglal a palyanak, majd inicializalja azt */
    bool ok;
    ok = foglal(pj);
    if(!ok)
        return false;
    inicializal(pj);

    return true;
}

/* SDL-hez szukseges idozito */
Uint32 idozito(Uint32 ms, void *param){
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

/* A jatek iranyitasaert felelos */
void jatekmenu(Megjelenites *pm, Jatek *pj){
    /* Palya bal felso keppontja */
    int palya_x = WINDOW_SZEL/2 - (pj->szel*MERET)/2;
    int palya_y = FEJLEC + (WINDOW_MAG-FEJLEC)/2 - pj->mag*MERET/2;
    /* A kattintas koordinataibol szamolt cellakoordinatakat tarolja */
    int x, y;
    /* SDL idozito, 1 masodpercig var, majd SDL_USERVENT-et ad az eventlistara */
    SDL_TimerID id = SDL_AddTimer(1000, idozito, NULL);
    /* Elso kirajzolas (hatter, fejlec, jatektabla) */
    boxRGBA(pm->renderer, 0, 0, WINDOW_SZEL-1, WINDOW_MAG-1, 0x6A, 0x73, 0x7B, 0xFF);
    fejlec(pm, pj);
    tabla_rajzol(pm, pj);
    /* Jatekido novelese */
    pj->ido++;

    /* Esemenyhurok */
    bool kilep = false;
    while(!kilep){
        bool felderites = false;
        bool jeloles = false;
        SDL_Event ev;
        SDL_WaitEvent(&ev);

        switch(ev.type){
            /* Masodpercenkent ujrarajzolja a fejlecet es noveli a jatekidot */
            case SDL_USEREVENT:
                fejlec(pm, pj);
                pj->ido++;
                break;
            /* Ertelmezi a kattintast */
            case SDL_MOUSEBUTTONDOWN:
                x = (ev.button.x - palya_x) / MERET;
                y = (ev.button.y - palya_y) / MERET;
                /* bal kattintas felderites */
                if(ev.button.button == SDL_BUTTON_LEFT)
                    felderites = true;
                /* jobb kattintas jeloles*/
                else if(ev.button.button == SDL_BUTTON_RIGHT)
                    jeloles = true;
                break;
            /* Kilepes */
            case SDL_QUIT:
                kilep = true;
                break;
        }

        /* Ha meg jatekban vagyunk, akkor meghivja
         * a felderites es jeloles fuggvenyeket,
         * majd kirajzolja a fejlecet es jatektablat */
        if(pj->vege == JATEKBAN){
            if(felderites)
                felderit(pj, x, y);
            else if(jeloles)
                jelol(pj, x, y);
            pj->vege = vege_van(pj);
            fejlec(pm, pj);
            tabla_rajzol(pm, pj);
        } else {
            /* Ha vege a jateknak toroljuk az idozitot, nincs mar ra szukseg
             * es megegyszer kirajzoljuk a fejlecet, illetve a tablat, de mar teljesen felfedve */
            SDL_RemoveTimer(id);
            fejlec(pm, pj);
            felfed(pm, pj);
        }

        /* Menti a jatekallast */
        mentes(pj);
    }
}
