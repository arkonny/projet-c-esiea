#ifndef MENU_ADMINISTRATION_H
#define MENU_ADMINISTRATION_H

#include <sqlite3.h>
#include <stdio.h>
#include "structs.h"
#include "debug.h"
#include "db_functions.h"
#include "fonctions.h"

// Menu livres
void rechercher_livre();

void emprunter_livre();

void retourner_livre();

void livres_empruntes();

void livres_disponibles();

void livres_totaux();

// Menu compte
void modifier_compte();

int supprimer_compte();

void afficher_compte();


// Menu administration
void ajouter_livre();

void supprimer_livre();

void rechercher_compte();

void ajouter_compte();

void supprimer_compte_admin();

#endif
