#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "db_functions.h"

sqlite3 *db;
Compte *currentUser;

int main(int argc, char **argv) {

	// Ouverture de la base de données
	int res = SQL_open();
	if (res) return err();

	// Initialisation du compte utilisateur
	currentUser = malloc(sizeof(Compte));


	/***********************************************
	 Si la base de données n'est pas initialisée
	 -> Création du compte administrateur
	 -> Initialisation des tables
	***********************************************/
	res = SQL_check_init();
	if (res == 1) {
		debug("La base de données n'était pas initialisée\n");
		// Création du compte utilisateur administateur
		char *id_user = "1";
		char *mot_de_passe = "admin";
		char *nom = "Admin";
		char *prenom = "Admin";
		char *mail = "admin@mail.com";
// --> Mot de passe à faire saisir par l'utilisateur
// (avec confirmation, et autres informations à saisir)
//
		sqlite3_user_add(db, id_user, mot_de_passe, strlen(mot_de_passe), 1);

		// Initialisation de la base de données
		res = SQL_init();
		init_Compte(currentUser, 1, nom, prenom, mail, 1);
		SQL_insertion_compte(currentUser);
		debug("Compte administrateur créé\n");
	}


	/************************************************
		La base de données est initialisée
		-> Connexion de l'utilisateur 
				(même si créé précédemment)
	************************************************/
	// Connexion de l'utilisateur
	user_connexion(currentUser);


	/************************************************
		Affichage du menu principal
		[1] 
		[2] 
		[3] 
		[4] 
		[5] 
		[6] 
		[7] 
		[8] Déconnexion
	************************************************/
	int choix = 0;
	

	// Boucle principale
	// -> Connexion
	// -> Menu principal
	// -> Déconnexion

	SQL_close();
  return 0;
}
