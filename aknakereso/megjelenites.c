#include "aknakereso.h"

#include "debugmalloc.h"

void sdl_init(Megjelenites *pm) {
    pm->ablak_mag = 640;
    pm->ablak_szel = 800;
    pm->ikon_meret = 16;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Aknakereső", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pm->ablak_szel, pm->ablak_mag, 0);
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
    TTF_Font *font = TTF_OpenFont("font/LiberationSerif-Regular.ttf", 32);
    if(font == NULL){
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }


    //SDL_RenderClear(renderer);

    pm->window = window;
    pm->renderer = renderer;
    pm->font = font;
}

void sdl_load_texture(Megjelenites *pm){
    pm->mezok = IMG_LoadTexture(pm->renderer, "img/texture.png");
    if(pm->mezok == NULL){
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
}

jatekmod fomenu(Megjelenites *pm){
    // fomenu krajzolasa
    /// free elni kell ezeket?????
    SDL_Surface *image = IMG_Load("img/fomenu.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(pm->renderer, image);
    SDL_RenderCopy(pm->renderer, texture, NULL, NULL);
    SDL_RenderPresent(pm->renderer);

    bool quit = false;
    while(!quit){
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type){
            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.y <= pm->ablak_mag / 2)
                    return UJ_JATEK;
                else
                    return BETOLTES;
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
    }
    // ExitSDL jon a helyere
    exit(1);
}

void mezo_rajzol(Megjelenites *pm, int x, int y) {
    SDL_Rect src = { (pm->melyik % 4) * MERET, (pm->melyik / 4) * MERET, MERET, MERET };
    SDL_Rect dest = { x, y, MERET, MERET };
    SDL_RenderCopy(pm->renderer, pm->mezok, &src, &dest);
}

void tabla_rajzol(Megjelenites *pm, Jatek *pj){
    int palya_x = WINDOW_SZEL/2 - pj->szel*MERET/2;
    int palya_y = WINDOW_MAG/2 - pj->mag*MERET/2;

    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].lathato){
                if(pj->palya[y][x].akna)
                    pm->melyik = 12;
                else
                    pm->melyik = pj->palya[y][x].ertek;

            } else if(pj->palya[y][x].jelolt){
                pm->melyik = 10;
            } else {
                pm->melyik = 9;
            }
            mezo_rajzol(pm, x * MERET + palya_x, y * MERET + palya_y);
        }
    }
    SDL_RenderPresent(pm->renderer);
}

void felfed(Megjelenites *pm, Jatek *pj){
    int palya_x = WINDOW_SZEL/2 - pj->szel*MERET/2;
    int palya_y = WINDOW_MAG/2 - pj->mag*MERET/2;
    for(int y = 0; y < pj->mag; ++y){
        for(int x = 0; x < pj->szel; ++x){
            if(pj->palya[y][x].akna)
                pm->melyik = 12;
            else
                pm->melyik = pj->palya[y][x].ertek;
            mezo_rajzol(pm, x * MERET + palya_x, y * MERET + palya_y);
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
    SDL_Rect text = { (pm->ablak_szel-pm->ablak_szel/2)/2, (pm->ablak_mag-40)/2, pm->ablak_szel/2, 40 };
    do{
        input_text(adat_str, 10, text, fekete, feher, pm->font, pm->renderer);
        adat_int = atoi(adat_str);
    } while(adat_int < min || adat_int > max);
    return adat_int;
}

void masodlagos_menu(Megjelenites *pm, Jatek *pj){
    int min, max;
    min = 5;
    max = pm->ablak_szel / 16;
    pj->szel = adat_beker(pm, min, max);
    min = 5;
    max = pm->ablak_mag / 16;
    pj->mag = adat_beker(pm, min, max);
    min = 1;
    max = pj->szel * pj->mag * 70/100;
    pj->akna_db = adat_beker(pm, min, max);
    SDL_RenderClear(pm->renderer);
}
