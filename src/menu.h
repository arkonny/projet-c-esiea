#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "debug.h"
#include "menu_fonctions.h"

// Saisie d'un entier
// pour le choix d'un menu
int selection(int nbChoix);

// Menu principal
void menu_principal();
void menu_principal_admin();
void menu_livres();
void menu_compte();
void menu_administration();

#endif
