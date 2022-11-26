#ifndef MENU_H
#define MENU_H

#include "rajzolo.h"
#include "fajlkezeles.h"

typedef enum jatekmod{ UJ_JATEK, BETOLTES }jatekmod;


int fomenu(Megjelenites *pm);
bool uj_jatek_menu(Megjelenites *pm, Jatek *pj);
void jatekmenu(Megjelenites *pm, Jatek *pj);

#endif // MENU_H
