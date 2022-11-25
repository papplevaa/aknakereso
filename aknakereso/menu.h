#ifndef MENU_H
#define MENU_H

#include "rajzolo.h"

typedef enum jatekmod{ UJ_JATEK, BETOLTES }jatekmod;

void sdl_init(Megjelenites *pm);
void sdl_close(Megjelenites *pm);

jatekmod fomenu(Megjelenites *pm);
bool uj_jatek_menu(Megjelenites *pm, Jatek *pj);
void jatekmenu(Megjelenites *pm, Jatek *pj);

#endif // MENU_H
