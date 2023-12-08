#include "fonctions.h"

// Saisie d'une chaîne de caractères
// Retourne la chaîne saisie
char *saisie_chaine(char *entree) {
		char *chaine = malloc(sizeof(char) * 100);
		printf("%s : ", entree);
		scanf("%s", chaine);
		return chaine;
}

// Clear une chaîne de caractères
// Prend en paramètre la chaîne à clear
void clear_chaine(char *chaine) {
	for (int i = 0; i < strlen(chaine); i++) {
		chaine[i] = 0;
	}
	free(chaine);
}

// Saisie d'une chaîne de caractères
// Faite deux fois pour confirmation
// Retourne la chaîne saisie
char *saisie_chaine_double(char *entree) {
	char *chaine, *chaine2;
	chaine = saisie_chaine(entree);
	chaine2 = saisie_chaine("Confirmation");

	while (strcmp(chaine, chaine2) != 0) {
		printf("Les entrées ne correspondent pas.\n");
		chaine = saisie_chaine(entree);
		chaine2 = saisie_chaine("Confirmation");
	}

	// Overwrite chaine2
	clear_chaine(chaine2);
	return chaine;
}


// Connexion de l'utilisateur
// Demande le mail
// Si pas de compte correspondant dans la base, inscription
// (l'inscription authentifie automatiquement l'utilisateur)
// Sinon, connexion
int user_connexion(Compte *user) {
	char *mail = saisie_chaine("Entrez votre mail");

	init_Compte(user, 0, "", "", mail, 0);

	int res = SQL_Compte_recherche(user);
	debug("Recherche de compte : %d\n", res);
	if (res != 100) {
		printf("Compte introuvable.\n");
		user_inscription(user);
	} else {
		char *mdp = saisie_chaine("Mot de passe");
		res = SQL_connexion(user, mdp);
		while(res != 0) {
			printf("Mot de passe incorrect.\n");
			mdp = saisie_chaine("Mot de passe");
			res = SQL_connexion(user, mdp);
		}
		clear_chaine(mdp);
		printf("Connexion réussie.\n");
	}
	return res;
}

int user_deconnexion(Compte *user) {
	init_Compte(user, 0, "", "", "", 0);
	debug("Déconnexion réussie.\n");
	return 0;
}


// Inscription d'un utilisateur (pas forcément currentUser)
// Demande nom, prénom, mail, mot de passe
// Propose admin si currentUser est admin
int user_inscription(Compte *user) {
	char *nom = saisie_chaine("Nom");
	char *prenom = saisie_chaine("Prénom");
	char *mdp = saisie_chaine_double("Mot de passe");

	int admin = 0;
	if(currentUser->admin == 1) {
		char *isAdmin = saisie_chaine("Admin ? (O/N)");
		if (strcmp(isAdmin, "O") == 0) {
			admin = 1;
		} 
		clear_chaine(isAdmin);
	} 

	init_Compte(user, 0, nom, prenom, user->mail, admin);
	int res = SQL_Compte_recherche(user);
	if (res == 0) {
		printf("Compte déjà existant.\n");
	} else {
		res = SQL_creation_compte(user, mdp);
		printf("Inscription réussie.\n");
		res = SQL_connexion(user, mdp);
	}

	clear_chaine(nom);
	clear_chaine(prenom);
	clear_chaine(mdp);

	return res;
}

// Affiche les informations d'un utilisateur
// Utilisateur courant par défaut
// Recherche utilisateur si currentUser est admin
int user_afficher() {
	return 0;
}
