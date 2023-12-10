#include "fonctions.h"

// Saisie d'une chaîne de caractères
// Retourne la chaîne saisie
char *saisie_chaine(char *entree) {
		char *chaine = malloc(101);
		printf("%s : ", entree);
		fgets(chaine, 100, stdin);
		chaine[strlen(chaine)-1] = '\0';
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
	char *chaine = saisie_chaine(entree);
	char *chaine2 = saisie_chaine("Confirmation");
	while (strcmp(chaine, chaine2) != 0) {
		printf("Les entrées ne correspondent pas.\n");
		chaine = saisie_chaine(entree);
		chaine2 = saisie_chaine("Confirmation");
	}
	return chaine;
}

// Saisie de la touche entrée
void saisie_entree() {
	getchar();
}

// Saisie binaire
// Retourne 1 si O, 0 si N
int saisie_binaire(char *entree) {
	char *chaine = saisie_chaine(entree);
	while (strcmp(chaine, "O") != 0 && strcmp(chaine, "N") != 0) {
		printf("Entrée invalide.\n");
		chaine = saisie_chaine(entree);
	}
	if (strcmp(chaine, "O") == 0) {
		return 1;
	} else {
		return 0;
	}
}

// Connexion de l'utilisateur
// Demande le mail
// Si pas de compte correspondant dans la base, inscription
// Sinon, connexion
int user_connexion(Compte *user) {
	char *mail = saisie_chaine("Entrez votre mail");
	init_Compte(user, 0, "", "", mail, 0);
	free(mail);
	int res = SQL_Compte_recherche(user);
	debug_Compte(user);
	debug("Recherche de compte : %d\n", res);
	if (!res) {
		printf("Compte introuvable.\n");
		if (saisie_binaire("Inscription ? (O/N)")) {
			res = user_inscription(user);
			res = user_connexion(user);
		} else {
			printf("Connexion annulée.\n");
			return -1;
		}
	} else {
		char *mdp = saisie_chaine("Mot de passe");
		res = SQL_connexion(user, mdp);
		debug("Connexion : %d\n", res);
		while(res != 0) {
			printf("Mot de passe incorrect.\n");
			if (!saisie_binaire("Réessayer ? (O/N)")) {
				printf("Connexion annulée.\n");
				clear_chaine(mdp);
				return -1;
			}
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
	char *mail;
	if (user->mail == NULL || strcmp(user->mail, "") == 0) {
		mail = saisie_chaine_double("Adresse mail");
	} else {
		mail = user->mail;
	}
	char *mdp = saisie_chaine_double("Mot de passe");

	int admin = 0;
	if(currentUser->admin == 1) {
		if (saisie_binaire("Administrateur ? (O/N)")) {
			admin = 1;
		} 
	} 

	init_Compte(user, 0, nom, prenom, mail, admin);
	int res = SQL_Compte_recherche(user);
	debug("Recherche de compte : %d\n", res);
	debug_Compte(user);
	if (res) {
		printf("Compte déjà existant.\n");
	} else {
		res = SQL_creation_compte(user, mdp);
		debug("Création de compte : %d\n", res);
		debug_Compte(user);
		debug("Mot de passe : %s\n", mdp);
		printf("Inscription réussie.\n");
	}

	free(nom);
	free(prenom);
	clear_chaine(mdp);
	return res;
}

// Affiche les informations d'un utilisateur
// Utilisateur courant par défaut
// Recherche utilisateur si currentUser est admin
int user_afficher() {
	return 0;
}
