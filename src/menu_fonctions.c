#include "menu_fonctions.h"

// Menu livres
void rechercher_livre() {
	char *entree = saisie_chaine("ISBN, titre, auteur ou genre");
	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, entree, entree, entree, entree, 0, "");
	free(entree);
	listeLivre *resultat = malloc(sizeof(listeLivre));
	init_listeLivre_vide(resultat);
	if (SQL_recherche(livre, resultat) == SQLITE_DONE) {
		afficher_listeLivre(resultat);
	} else {
		printf("Livre non trouvé\n");
	}
	liberer_listeLivre(resultat);
	free(livre);
}

void emprunter_livre() {
	char *isbn = saisie_chaine("ISBN");
	Livre *livre = malloc(sizeof(Livre));
	char *date = date_actuelle();
	init_Livre(livre, isbn, "", "", "", currentUser->id_user, date);
	free(date);
	free(isbn);
	if (SQL_emprunt(livre) == SQLITE_DONE) {
		printf("Livre emprunté avec succès !\n");
	} else {
		printf("Erreur lors de l'emprunt du livre !\n");
	}
	free(livre);
}

void retourner_livre() {
	char *isbn = saisie_chaine("ISBN");
	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, isbn, "", "", "", 0, "");
	free(isbn);
	if (SQL_retour(livre) == SQLITE_DONE) {
		printf("Livre retourné avec succès !\n");
	} else {
		printf("Erreur lors du retour du livre !\n");
	}
	free(livre);
}

void livres_empruntes() {
	listeLivre *resultat = malloc(sizeof(listeLivre));
	init_listeLivre_vide(resultat);
	if (SQL_livres_empruntes(currentUser, resultat) == SQLITE_DONE) {
		afficher_listeLivre(resultat);
	} else {
		printf("Erreur lors de la récupération des livres empruntés\n");
	}
	liberer_listeLivre(resultat);
}

void livres_disponibles() {
	printf("Fonctionnalité non implémentée\n");
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
		printf("Livre ajouté avec succès !\n");
	} else {
		printf("Erreur lors de l'ajout du livre !\n");
	}

	free(livre);
}

void supprimer_livre() {
	char *isbn = saisie_chaine("ISBN");
	Livre *livre = malloc(sizeof(Livre));
	init_Livre(livre, isbn, "", "", "", 0, "");
	free(isbn);

	if (SQL_suppression(livre) == 0) {
		printf("Livre supprimé avec succès\n");
	} else {
		printf("Erreur lors de la suppression du livre\n");
	}
	free(livre);
}

void rechercher_compte() {
	char *mail = saisie_chaine("Mail");
	Compte *compte = malloc(sizeof(Compte));
	init_Compte(compte, 0, "", "", mail, "", 0);
	free(mail);

	if (SQL_Compte_recherche(compte)) {
		print_Compte(compte);
	} else {
		printf("Compte non trouvé\n");
	}
	free(compte);
}

void ajouter_compte() {
	Compte *compte = malloc(sizeof(Compte));
	init_Compte(compte, 0, "", "", "", "", 0);
	int res = user_inscription(compte);
	if (res) {
		printf("Compte ajouté avec succès !\n");
	} else {
		printf("Erreur lors de l'ajout du compte !\n");
	}
	free(compte);
}

void supprimer_compte_admin() {
	char *mail = saisie_chaine("Mail");
	Compte *compte = malloc(sizeof(Compte));
	init_Compte(compte, 0, "", "", mail, "", 0);
	free(mail);

	if (!SQL_Compte_recherche(compte)) {
		printf("Compte introuvable\n");
		free(compte);
		return;
	}

	if (SQL_suppression_compte(compte) == SQLITE_DONE) {
		printf("Compte supprimé avec succès\n");
	} else {
		printf("Erreur lors de la suppression du compte\n");
	}
	free(compte);
}

void modifier_compte_admin() {
	printf("Fonctionnalité non implémentée\n");
}
