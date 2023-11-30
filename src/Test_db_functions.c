#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
//#include "structs.h"
#include "db_functions.h"

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

sqlite3 *db;

// Main function tests each function of db_functions.c
int main() {
	int res;

	res = SQL_init();
	printf("SQL_init() returned %d\n", res);
	if (res) {
		printf("SQL_init() fail\n");
		return 1;
	}

	res = SQL_open();
	printf("SQL_open() returned %d\n", res);
	if (res) {
		printf("SQL_open() fail\n");
		return 1;
	}

	Livre book = {"xxxxxxxxxxxxx","Book Title", "Author","Genre",1,1,""}; 

	res = SQL_ajout(&book);
	printf("SQL_ajout(book) returned %d\n", res);
	if (res) {
		printf("SQL_ajout(book) fail\n");
		return 1;
	}

	res = SQL_close();
	printf("SQL_close() returned %d\n", res);
	if (res) {
		printf("SQL_close() fail\n");
		return 1;
	}
}
