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

	// Affichage du titre
	printf("|=============================|\n");
	printf("                               \n");
	printf("       Librairie SQLite3       \n");
	printf("         - Projet C -          \n");
	printf("                               \n");
	printf("|=============================|\n\n");

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
		printf("La base de données n'était pas initialisée\n");
		saisie_entree();
		// Création du compte utilisateur administateur
		print_titre("Création du compte administrateur");
		printf("Entrez les informations du compte\n");
		char *nom = saisie_chaine("Nom");
		char *prenom = saisie_chaine("Prénom");
		char *mail = saisie_chaine_double("Adresse mail");
		char *mdp = saisie_chaine_double("Mot de passe");
		char *mdp_hash = hash(mdp, 1);

		// Initialisation de la base de données
		res = SQL_init();
		init_Compte(currentUser, 1, nom, prenom, mail, mdp_hash, 1);
		SQL_insertion_compte(currentUser);
		print_retour("Compte administrateur créé");
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
	//free(currentUser);

	SQL_close();
  return 0;
}
