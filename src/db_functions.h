#ifndef DB_FUNCTIONS_H
#define DB_FUNCTIONS_H

#include <sqlite3.h>
#include <sqlite3userauth.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "debug.h"

// La variable db est globale et est utilisée par toutes les fonctions
extern sqlite3 *db;

/*********************************************
* Fonctions de gestion de la base de données *
*********************************************/
// La base de donnée est ouverte une seule fois au lancement du programme
// et fermée à la fin de celui-ci
int SQL_init();
int SQL_check_init();
int SQL_open();
int SQL_close();


/*************************
* Fonctions de recherche *
*************************/
// Les objets de type Livre et Compte ne sont pas forcément complets
// d'où leur utilisation pour des fonctions de création de compte et de livre
int SQL_recherche(Livre *livre, listeLivre *liste);

// Retourne une liste de livres empruntés par un utilisateur
int SQL_livres_empruntes(Compte *user, listeLivre *liste);

// Complète un objet Livre avec les informations de la base de données
int SQL_disponibilite(Livre *livre);


/**********************************
* Fonctions de gestion des livres *
**********************************/
int SQL_ajout(Livre *livre);
int SQL_emprunt(Livre *livre, Compte *user);
int SQL_retour(Livre *livre);
int SQL_suppression(Livre *livre);


/*********************************
* Fonctions de gestion de compte *
*********************************/
int SQL_Compte_recherche(Compte *user);
int SQL_insertion_compte(Compte *user);
int SQL_creation_compte(Compte *user, char *mot_de_passe);
int SQL_connexion(Compte *user, char *mot_de_passe);
int SQL_changement_mdp(Compte *user, char *mot_de_passe);
int SQL_changement_mail(Compte *user, char *new_mail);
int SQL_suppression_compte(Compte *user);

#endif
