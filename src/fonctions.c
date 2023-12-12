#include "fonctions.h"

// Lit une ligne de la console
// Utilisation de getchar() uniquement
// Pour compatibilité Windows
void gets_s(char *chaine, int taille) {
	int i = 0;
	char c = getchar();
	while (c != '\n' && i < taille) {
		chaine[i] = c;
		i++;
		c = getchar();
	}
	chaine[i] = '\0';
}

// Saisie d'une chaîne de caractères
// Retourne la chaîne saisie
char *saisie_chaine(char *entree) {
		char *chaine = malloc(101);
		printf("%s -> ", entree);
		gets_s(chaine, 100);
		//fgets(chaine, 100, stdin);
		//chaine[strlen(chaine)-1] = '\0';
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
	printf("[Entrée pour continuer]");
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
	char *salt_str = malloc(10);
	sprintf(salt_str, "$1$%d$", salt);
	char *hash = crypt(mdp, salt_str);
	debug("Mdp : %s\n", mdp);
	debug("Salt : %s\n", salt_str);
	debug("Hash : %s\n", hash);
	free(salt_str);
	return hash;
}

char *date_actuelle() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *date = malloc(11);
	sprintf(date, "%d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	return date;
}

// Fonction d'affichage de l'action en cours
// Prend en paramètre un titre
// Affiche le titre centré dans un cadre
void print_titre(char *titre) {
	printf("\n\n");
	int taille = strlen(titre);
	for (int i = 0; i < 30 - taille/2; i++) {
		printf("=");
	}
	printf("| %s |", titre);
	for (int i = 0; i < 30 - taille/2; i++) {
		printf("=");
	}
	printf("\n");
}

void print_retour(char *titre) {
	printf("|=> %s\n", titre);
}


// Connexion de l'utilisateur
// Demande le mail
// Si pas de compte correspondant dans la base, inscription
// Sinon, connexion
int user_connexion(Compte *user) {
	print_titre("Connexion");
	char *mail = saisie_chaine("Entrez votre mail");
	init_Compte(user, 0, "", "", mail, "", 0);
	free(mail);
	int res = SQL_Compte_recherche(user);
	debug_Compte(user);
	debug("Recherche de compte : %d\n", res);
	if (!res) {
		print_retour("Compte introuvable.\n");
		if (saisie_binaire("Inscription ? [O/N]")) {
			res = user_inscription(user);
			if (res == -1) {
				print_retour("Inscription annulée.\n");
				return -1;
			} else {
				res = user_connexion(user);
			}
		} else {
			print_retour("Connexion annulée.\n");
			return -1;
		}
	} else {
		char *mdp = saisie_chaine("Mot de passe");
		char *mdp_hash = hash(mdp, user->id_user);
		res = strcmp(mdp_hash, user->hash);
		while(res != 0) {
			printf("Mot de passe incorrect.\n");
			if (!saisie_binaire("Réessayer ? [O/N]")) {
				print_retour("Connexion annulée.\n");
				return -1;
			}
			clear_chaine(mdp);
			mdp = saisie_chaine("Mot de passe");
			mdp_hash = hash(mdp, user->id_user);
			res = strcmp(mdp_hash, user->hash);
		}
		clear_chaine(mdp);
		print_retour("Connexion réussie.\n");
	}
	return res;
}

int user_deconnexion(Compte *user) {
	init_Compte(user, 0, "", "", "", "", 0);
	print_retour("Déconnexion réussie\n");
	return 0;
}


// Inscription d'un utilisateur (pas forcément currentUser)
// Demande nom, prénom, mail, mot de passe
// Propose admin si currentUser est admin
int user_inscription(Compte *user) {
	int admin = 0;
	if(currentUser->admin == 1) {
		if (saisie_binaire("Administrateur ? [O/N]")) {
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
		print_retour("Compte déjà existant.\n");
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
