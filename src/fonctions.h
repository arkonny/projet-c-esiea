#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

// Saisie d'une chaîne de caractères
// Retourne la chaîne saisie
char *saisie_chaine();

// Connexion de l'utilisateur
// Demande le mail
// Si pas de compte correspondant dans la base, inscription
// (l'inscription authentifie automatiquement l'utilisateur)
// Sinon, connexion
user_connexion(Compte *user);

// Inscription d'un utilisateur (pas forcément currentUser)
// Demande nom, prénom, mail, mot de passe
// Propose admin si currentUser est admin
user_inscription(Compte *user);

// Affiche les informations d'un utilisateur
// Utilisateur courant par défaut
// Recherche utilisateur si currentUser est admin
user_afficher();

#endif
