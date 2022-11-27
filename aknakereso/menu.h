#ifndef MENU_H
#define MENU_H

#include "rajzolo.h"
#include "fajlkezeles.h"

/* Jatekmod */
enum { UJ_JATEK = 0, BETOLTES = 1 }jatekmod;

int fomenu(Megjelenites *pm);
bool uj_jatek_menu(Megjelenites *pm, Jatek *pj);
void jatekmenu(Megjelenites *pm, Jatek *pj);

#endif // MENU_H
