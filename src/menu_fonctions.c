#include "menu_fonctions.h"

// Menu livres
void rechercher_livre() {
	print_titre("Recherche de livres");
	char *entree = saisie_chaine("ISBN, titre, auteur ou genre");
	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, entree, entree, entree, entree, -1, "-1");
	free(entree);
	listeLivre *resultat = malloc(sizeof(listeLivre));
	init_listeLivre_vide(resultat);
	if (SQL_recherche(livre, resultat)) {
		print_retour("Livre(s) trouvé(s)\n");
		afficher_listeLivre(resultat);
	} else {
		print_retour("Aucun livre trouvé\n");
	}
	liberer_listeLivre(resultat);
	free(livre);
}

void emprunter_livre() {
	print_titre("Emprunt de livre");
	char *isbn = saisie_chaine("ISBN");
	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, isbn, "", "", "", -1, "-1");
	if (!SQL_recherche(livre, NULL)) {
		printf("Livre introuvable\n");
	} else if (livre->id_user != 0) {
		printf("Livre déjà emprunté\n");
	} else {
		char *date = date_actuelle();
		init_Livre(livre, isbn, "", "", "", currentUser->id_user, date);
		if (SQL_emprunt(livre) == SQLITE_DONE) {
			printf("Livre emprunté avec succès !\n");
		} else {
			printf("Erreur lors de l'emprunt du livre !\n");
		}
		free(date);
	}
	free(isbn);
	free(livre);
}

void retourner_livre() {
	print_titre("Retour de livre");
	char *isbn = saisie_chaine("ISBN");
	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, isbn, "", "", "", -1, "-1");
	free(isbn);
	if (!SQL_recherche(livre, NULL)) {
		printf("Livre introuvable\n");
	} else if (livre->id_user == 0) {
		printf("Livre déjà disponible\n");
	} else if (livre->id_user != currentUser->id_user) {
		printf("Vous n'avez pas emprunté ce livre\n");
	} else {
		init_Livre(livre, isbn, "", "", "", 0, "");
		if (SQL_retour(livre) == SQLITE_DONE) {
			printf("Livre retourné avec succès !\n");
		} else {
			printf("Erreur lors du retour du livre !\n");
		}
	}
	free(livre);
}

void livres_empruntes() {
	print_titre("Livres empruntés");
	listeLivre *resultat = malloc(sizeof(listeLivre));
	init_listeLivre_vide(resultat);
	if (SQL_livres_empruntes(currentUser, resultat) == SQLITE_DONE) {
		if (resultat->taille == 0) {
			printf("Vous n'avez emprunté aucun livre\n");
		} else {
			afficher_listeLivre(resultat);
		}
	} else {
		printf("Erreur lors de la récupération des livres empruntés\n");
	}
	liberer_listeLivre(resultat);
}

// En recherchant les livres empruntés par l'utilisateur 0, on obtient les livres disponibles
void livres_disponibles() {
	print_titre("Livres disponibles");
	listeLivre *resultat = malloc(sizeof(listeLivre));
	init_listeLivre_vide(resultat);
	Compte *user = malloc(sizeof(Compte));
	init_Compte(user, 0, "", "", "", "", 0);
	if (SQL_livres_empruntes(user, resultat) == SQLITE_DONE) {
		if (resultat->taille == 0) {
			printf("Aucun livre n'est disponible\n");
		} else {
			afficher_listeLivre(resultat);
		}
	} else {
		printf("Erreur lors de la récupération des livres disponibles\n");
	}
	free(user);
	liberer_listeLivre(resultat);
}

void livres_totaux() {
	print_titre("Livres de la base");
	listeLivre *resultat = malloc(sizeof(listeLivre));
	init_listeLivre_vide(resultat);
	if (!SQL_livres_totaux(resultat)) {
		if (resultat->taille == 0) {
			printf("Aucun livre dans la base\n");
		} else {
			afficher_listeLivre(resultat);
		}
	} else {
		printf("Erreur lors de la récupération des livres disponibles\n");
	}
	liberer_listeLivre(resultat);
}

// Menu compte
void modifier_compte() {
	printf("Fonctionnalité non implémentée\n");
}

void supprimer_compte() {
	printf("Fonctionnalité non implémentée\n");
}

void afficher_compte() {
	printf("ID | Nom       | Prénom    | Mail      | Admin\n");
	print_Compte(currentUser);
}


// Menu administrateur

void ajouter_livre() {
	print_titre("Ajout de livre");
	char *titre = saisie_chaine("Titre");
	char *auteur = saisie_chaine("Auteur");
	char *genre = saisie_chaine("Genre");
	char *isbn = saisie_chaine("ISBN");

	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, isbn, titre, auteur, genre, 0, "");
	free(titre);
	free(auteur);
	free(genre);
	free(isbn);

	if (SQL_ajout(livre) == SQLITE_DONE) {
		print_retour("Livre ajouté avec succès !\n");
	} else {
		print_retour("Erreur lors de l'ajout du livre !\n");
	}

	free(livre);
}

void supprimer_livre() {
	print_titre("Suppression de livre");
	char *isbn = saisie_chaine("ISBN");
	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, isbn, "", "", "", 0, "");
	free(isbn);

	if (SQL_suppression(livre) == 0) {
		print_retour("Livre supprimé avec succès\n");
	} else {
		print_retour("Erreur lors de la suppression du livre\n");
	}
	free(livre);
}

void rechercher_compte() {
	print_titre("Recherche de compte");
	char *mail = saisie_chaine("Mail");
	Compte *compte = malloc(sizeof(Compte));
	init_Compte(compte, 0, "", "", mail, "", 0);
	free(mail);

	if (SQL_Compte_recherche(compte)) {
		print_retour("Compte trouvé\n");
		print_Compte(compte);
	} else {
		print_retour("Compte non trouvé\n");
	}
	free(compte);
}

void ajouter_compte() {
	print_titre("Ajout de compte");
	Compte *compte = malloc(sizeof(Compte));
	init_Compte(compte, 0, "", "", "", "", 0);
	int res = user_inscription(compte);
	if (res) {
		print_retour("Compte ajouté avec succès !\n");
	} else {
		print_retour("Erreur lors de l'ajout du compte !\n");
	}
	free(compte);
}

void supprimer_compte_admin() {
	print_titre("Suppression de compte");
	char *mail = saisie_chaine("Mail");
	Compte *compte = malloc(sizeof(Compte));
	init_Compte(compte, 0, "", "", mail, "", 0);
	free(mail);

	if (!SQL_Compte_recherche(compte)) {
		print_retour("Compte introuvable\n");
		free(compte);
		return;
	}

	if (SQL_suppression_compte(compte) == SQLITE_DONE) {
		print_retour("Compte supprimé avec succès\n");
	} else {
		print_retour("Erreur lors de la suppression du compte\n");
	}
	free(compte);
}

void modifier_compte_admin() {
	printf("Fonctionnalité non implémentée\n");
}
