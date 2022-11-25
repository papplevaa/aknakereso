#include "menu.h"

#include "debugmalloc.h"

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

    pm->window = window;
    pm->renderer = renderer;
    pm->font = font;
}

void sdl_close(Megjelenites *pm){
    TTF_CloseFont(pm->font);
    pm->font = NULL;

    SDL_DestroyRenderer(pm->renderer);
    pm->renderer = NULL;

    SDL_DestroyWindow(pm->window);
    pm->window = NULL;

    SDL_Quit();
}

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

jatekmod fomenu(Megjelenites *pm){
    // hatterszin
    hatter(pm);
    szoveg(pm, "Új játék", 0, WINDOW_MAG/2);
    szoveg(pm, "Betöltés", WINDOW_MAG/2, WINDOW_MAG/2);
    vonal(pm);

    // renderer uritese
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
    hatter(pm);
    sprintf(str, "Pálya szélessége (%d..%d):", min, max);
    szoveg(pm, str, WINDOW_MAG/4-40, WINDOW_MAG/2);
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->szel = adat;
    SDL_RenderClear(pm->renderer);

    min = 5;
    max = (WINDOW_MAG-32) / MERET;
    hatter(pm);
    sprintf(str, "Pálya magassága (%d..%d):", min, max);
    szoveg(pm, str, WINDOW_MAG/4-40, WINDOW_MAG/2);
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->mag = adat;
    SDL_RenderClear(pm->renderer);

    min = 5;
    max = pj->szel * pj->mag * 30/100;
    hatter(pm);
    sprintf(str, "Aknák száma (%d..%d):", min, max);
    szoveg(pm, str, WINDOW_MAG/4-40, WINDOW_MAG/2);
    adat = adat_beker(pm, min, max);
    if(adat == -1)
        return false;
    pj->akna_db = adat;

    SDL_RenderClear(pm->renderer);
    return true;
}

void jatekmenu(Megjelenites *pm, Jatek *pj){
    // hatter
    hatter(pm);

    // textura betoltese
    pm->mezok = IMG_LoadTexture(pm->renderer, "img/texture.png");
    if(pm->mezok == NULL){
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    // kirajzolja az ures tablat
    tabla_rajzol(pm, pj);

    // esemenyhurok
    bool quit = false;
    int x, y;

    int palya_x = WINDOW_SZEL/2 - (pj->szel*MERET)/2;
    int palya_y = WINDOW_MAG/2 - (pj->mag*MERET)/2;

    while(!quit){
        bool felderites = false;
        bool jeloles = false;
        SDL_Event ev;
        SDL_WaitEvent(&ev);

        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                x = (ev.button.x - palya_x) / MERET;
                y = (ev.button.y - palya_y) / MERET;
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

    // textura felszabaditasa
    SDL_DestroyTexture(pm->mezok);
}
