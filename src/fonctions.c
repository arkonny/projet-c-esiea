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

char *hash(char *mdp, int salt) {
	char *charsalt = malloc(2);
	sprintf(charsalt, "$%d$", salt);
	char *hash = crypt(mdp, "$6$");
	debug("Mdp : %s\n", mdp);
	debug("Salt : %s\n", charsalt);
	debug("Hash : %s\n", hash);
	//free(charsalt);
	return hash;
}


// Connexion de l'utilisateur
// Demande le mail
// Si pas de compte correspondant dans la base, inscription
// Sinon, connexion
int user_connexion(Compte *user) {
	char *mail = saisie_chaine("Entrez votre mail");
	init_Compte(user, 0, "", "", mail, "", 0);
	free(mail);
	int res = SQL_Compte_recherche(user);
	debug_Compte(user);
	debug("Recherche de compte : %d\n", res);
	if (!res) {
		printf("Compte introuvable.\n");
		if (saisie_binaire("Inscription ? (O/N)")) {
			res = user_inscription(user);
			if (res == -1) {
				printf("Inscription annulée.\n");
				return -1;
			} else {
				res = user_connexion(user);
			}
		} else {
			printf("Connexion annulée.\n");
			return -1;
		}
	} else {
		char *mdp = saisie_chaine("Mot de passe");
		char *mdp_hash = hash(mdp, user->id_user);
		res = strcmp(mdp_hash, user->hash);
		while(res != 0) {
			printf("Mot de passe incorrect.\n");
			if (!saisie_binaire("Réessayer ? (O/N)")) {
				printf("Connexion annulée.\n");
				return -1;
			}
			clear_chaine(mdp);
			mdp = saisie_chaine("Mot de passe");
			mdp_hash = hash(mdp, user->id_user);
			res = strcmp(mdp_hash, user->hash);
		}
		clear_chaine(mdp);
		printf("Connexion réussie.\n");
	}
	return res;
}

int user_deconnexion(Compte *user) {
	init_Compte(user, 0, "", "", "", "", 0);
	debug("Déconnexion réussie.\n");
	return 0;
}


// Inscription d'un utilisateur (pas forcément currentUser)
// Demande nom, prénom, mail, mot de passe
// Propose admin si currentUser est admin
int user_inscription(Compte *user) {
	int admin = 0;
	if(currentUser->admin == 1) {
		if (saisie_binaire("Administrateur ? (O/N)")) {
			admin = 1;
		} 
	} 
	char *nom = saisie_chaine("Nom");
	char *prenom = saisie_chaine("Prénom");
	if (user->mail == NULL || strcmp(user->mail, "") == 0) {
		char *mail = saisie_chaine_double("Adresse mail");
		init_Compte(user, 0, nom, prenom, mail, "", admin);
		free(mail);
	} else {
		init_Compte(user, 0, nom, prenom, user->mail, "", admin);
	}
	char *mdp = saisie_chaine_double("Mot de passe");

	int res = SQL_Compte_recherche(user);
	debug("Recherche de compte : %d\n", res);
	debug_Compte(user);
	if (res) {
		printf("Compte déjà existant.\n");
		res = -1;
	} else {
		res = SQL_insertion_compte(user);
		char *mdp_hash = hash(mdp, user->id_user);
		res = SQL_changement_mdp(user, mdp_hash);
		debug_Compte(user);
	}

	free(nom);
	free(prenom);
	clear_chaine(mdp);
	debug("user_inscription : %d\n", res);
	return res;
}

// Affiche les informations d'un utilisateur
// Utilisateur courant par défaut
// Recherche utilisateur si currentUser est admin
int user_afficher() {
	return 0;
}
