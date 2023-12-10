#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include "structs.h"
#include "debug.h"
#include "db_functions.h"

// Saisie d'une chaîne de caractères
// Retourne la chaîne saisie
char *saisie_chaine(char *entree);

// Saisie d'un mot de passe
// Retourne le mot de passe saisi
char *saisie_chaine_double(char *entree);

// Saisie de n'importe quelle touche pour continuer
void saisie_entree();

// Saisie binaire (oui/non)
int saisie_binaire();

// Hashage d'une chaîne de caractères
// Retourne le hash
char *hash(char *mdp, int salt);

// Connexion de l'utilisateur
// Demande le mail
// Si pas de compte correspondant dans la base, inscription
// (l'inscription authentifie automatiquement l'utilisateur)
// Sinon, connexion
int user_connexion(Compte *user);
int user_deconnexion(Compte *user);

// Inscription d'un utilisateur (pas forcément currentUser)
// Demande nom, prénom, mail, mot de passe
// Propose admin si currentUser est admin
int user_inscription(Compte *user);

// Affiche les informations d'un utilisateur
// Utilisateur courant par défaut
// Recherche utilisateur si currentUser est admin
int user_afficher();

#endif
