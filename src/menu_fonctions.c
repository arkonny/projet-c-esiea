#include "menu_fonctions.h"

// Menu livres
void rechercher_livre() {
	print_titre("Recherche de livres");
	char *saisie = malloc(101);
	printf("ISBN, titre, auteur ou genre\n(ou laisser vide pour afficher tous les livres) -> ");
	gets_s(saisie, 100);
	char *entree = malloc(sizeof(char) * (strlen(saisie) + 3));
	sprintf(entree, "%%%s%%", saisie);
	free(saisie);
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
		print_retour("Livre introuvable\n");
	} else if (livre->id_user != 0 && livre->id_user != currentUser->id_user) {
		print_retour("Livre déjà emprunté par un autre utilisateur\n");
	} else if (livre->id_user == currentUser->id_user) {
		print_retour("Vous avez déjà emprunté ce livre\n");
	} else {
		char *date = date_actuelle();
		init_Livre(livre, isbn, "", "", "", currentUser->id_user, date);
		if (SQL_emprunt(livre) == SQLITE_DONE) {
			print_retour("Livre emprunté avec succès !\n");
		} else {
			print_retour("Erreur lors de l'emprunt du livre !\n");
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
		print_retour("Livre introuvable\n");
	} else if (livre->id_user == 0) {
		print_retour("Livre déjà disponible\n");
	} else if (livre->id_user != currentUser->id_user) {
		print_retour("Vous n'avez pas emprunté ce livre\n");
	} else {
		if (SQL_retour(livre) == SQLITE_DONE) {
			print_retour("Livre retourné avec succès !\n");
		} else {
			print_retour("Erreur lors du retour du livre !\n");
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
			print_retour("Aucun livre dans la base\n");
		} else {
			afficher_listeLivre(resultat);
		}
	} else {
		print_retour("Erreur lors de la récupération des livres disponibles\n");
	}
	liberer_listeLivre(resultat);
}

// Menu compte
void modifier_compte() {
	print_titre("Modification de compte");
	if (saisie_binaire("Voulez-vous changer votre mail ? [O/N]")) {
		char *new_mail = saisie_chaine_double("Nouveau mail");
		if (SQL_changement_mail(currentUser, new_mail) == SQLITE_DONE) {
			print_retour("Mail changé avec succès\n");
		} else {
			print_retour("Erreur lors du changement de mail\n");
		}
		free(new_mail);
	}
	if (saisie_binaire("Voulez-vous changer votre mot de passe ? [O/N]")) {
		char *new_mdp = saisie_chaine_double("Nouveau mot de passe");
		char *new_hash = hash(new_mdp, currentUser->id_user);
		if (SQL_changement_mdp(currentUser, new_hash) == SQLITE_DONE) {
			print_retour("Mot de passe changé avec succès\n");
		} else {
			print_retour("Erreur lors du changement de mot de passe\n");
		}
		free(new_mdp);
	}
}

int supprimer_compte() {
	print_titre("Suppression de compte");
	if (saisie_binaire("Voulez-vous vraiment supprimer votre compte ? [O/N]")) {
		if (SQL_suppression_compte(currentUser) == SQLITE_DONE) {
			print_retour("Compte supprimé avec succès\n");
			return 1;
		} else {
			print_retour("Erreur lors de la suppression du compte\n");
		}
	}
	return 0;
}

void afficher_compte() {
	printf("ID | Nom                 | Prénom         | Mail                | Admin\n");
	printf("---|---------------------|----------------|---------------------|------\n");
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

	if (SQL_suppression(livre) == SQLITE_DONE) {
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
		printf("ID | Nom                 | Prénom         | Mail                | Admin\n");
		printf("---|---------------------|----------------|---------------------|------\n");
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
	listeLivre *resultat = malloc(sizeof(listeLivre));
	SQL_livres_empruntes(compte, resultat);
	if (resultat->taille > 0) {
		print_retour("Ce compte a encore des livres empruntés\n");
		liberer_listeLivre(resultat);
		free(compte);
		return;
	}
	liberer_listeLivre(resultat);

	if (SQL_suppression_compte(compte) == SQLITE_DONE) {
		print_retour("Compte supprimé avec succès\n");
	} else {
		print_retour("Erreur lors de la suppression du compte\n");
	}
	free(compte);
}
