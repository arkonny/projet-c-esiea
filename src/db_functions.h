#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <structs.h>

// Insert parameters in a query
int db_insert_query(sqlite3 *db, char *sql_insert, int nbr_of_parameters,
                    char *parameters_names[], char *parameters[],
                    sqlite3_stmt *res);

// Insert a book
int db_insert_book(sqlite3 *db, int ISBN, char *Titre, char *Auteur,
                   char *Genre);

// Fonctions de gestion de la base de données
// La base de donnée est ouverte une seule fois au lancement du programme
// et fermée à la fin de celui-ci
// La variable db est globale et est utilisée par toutes les fonctions
int SQL_init();
int SQL_open();
int SQL_close();

// Les objets de type Livre et Compte ne sont pas forcément complets
// d'où leur utilisation pour des fonctions de création de compte et de Livre

// Fonctions de recherche
int SQL_recherche(char *entree);
int SQL_livres_empruntes(Compte *user);
int SQL_disponibilite(Livre *livre);

// Fonctions de modification
int SQL_ajout(Livre *livre);
int SQL_emprunt(Livre *livre, Compte *user);
int SQL_retour(Livre *livre, Compte *user);
int SQL_suppression(Livre *livre);

// Fonctions de gestion de compte
int SQL_creation_compte(Compte *user);
int SQL_connexion(Compte *user);
int SQL_deconnexion(Compte *user);
int SQL_suppression_compte(Compte *user);
