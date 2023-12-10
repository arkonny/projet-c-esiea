#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "debug.h"
#include "menu.h"
#include "fonctions.h"
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
		printf("Entrez les informations du compte administrateur\n");
		char *nom = saisie_chaine("Nom");
		char *prenom = saisie_chaine("Prénom");
		char *mail = saisie_chaine_double("Adresse mail");
		char *mdp = saisie_chaine_double("Mot de passe");
		char *mdp_hash = hash(mdp, 1);

		// Initialisation de la base de données
		res = SQL_init();
		init_Compte(currentUser, 1, nom, prenom, mail, mdp_hash, 1);
		SQL_insertion_compte(currentUser);
		printf("Compte administrateur créé\n");
		free(nom);
		free(prenom);
		free(mail);
		free(mdp);
	}


	/************************************************
		La base de données est initialisée
		-> Connexion de l'utilisateur
		-> Boucle principale
			|-> Affichage du menu principal
			|-> Traitement du choix de l'utilisateur
		-> Déconnexion de l'utilisateur
	************************************************/

	// Connexion de l'utilisateur
	res = user_connexion(currentUser);
	if (res == -1) {
		SQL_close();
		return 0;
	}

	// Boucle principale
	if (currentUser->admin) {
		menu_principal_admin();
	} else {
		menu_principal();
	}
	
	// Déconnexion de l'utilisateur
	user_deconnexion(currentUser);

	SQL_close();
  return 0;
}
