#ifndef MENU_H
#define MENU_H

#include "rajzolo.h"

typedef enum jatekmod{ UJ_JATEK, BETOLTES }jatekmod;

void sdl_init(Megjelenites *pm);
void sdl_load_texture(Megjelenites *pm);
//void sdl_close(Megjelenites *pm);

jatekmod fomenu(Megjelenites *pm);
// kovi kettot egybegyurni
void masodlagos_menu(Megjelenites *pm, Jatek *pj);
int adat_beker(Megjelenites *pm, int min, int max);
void jatekmenu(Megjelenites *pm, Jatek *pj);

#endif // MENU_H
