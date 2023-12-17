#include "menu.h"

// Saisie d'un entier
// pour le choix d'un menu
int selection(int nbChoix) {
	int choix;
	printf("Votre choix : ");
	scanf("%d", &choix);
	while(choix < 1 || choix > nbChoix) {
		// Clear stdin
		int c;
		while ((c = getchar()) != '\n' && c != EOF) { }
		choix = 0;

		printf("Choix invalide\n");
		printf("Un nombre entre 1 et %d est attendu\n", nbChoix);
		printf("Votre choix : ");
		scanf("%d", &choix);
	}
	// Clear stdin
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
	return choix;
}

// Menu principal
void affichage_menu_principal() {
	print_titre_menu("Menu principal");
	char *menu = 
		"[1]. Livres\n"
		"[2]. Compte\n"
		"[3]. Quitter\n";
	printf("%s", menu);
}

void menu_principal() {
	int choix = 0;
	while(choix != 3) {
		affichage_menu_principal();
		choix = selection(3);
		switch(choix) {
			case 1:
				menu_livres();
				break;
			case 2:
				menu_compte();
				break;
			case 3:
				break;
			default:
				printf("Erreur\n");
				break;
		}
	}
}

void affichage_menu_principal_admin() {
	print_titre_menu("Menu principal");
	char *menu =
		"[1]. Livres\n"
		"[2]. Compte\n"
		"[3]. Administration\n"
		"[4]. Quitter\n";
	printf("%s", menu);
}

void menu_principal_admin() {
	int choix = 0;
	while(choix != 4) {
		affichage_menu_principal_admin();
		choix = selection(4);
		switch(choix) {
			case 1:
				menu_livres();
				break;
			case 2:
				if (menu_compte()) {
					return;
				}
				break;
			case 3:
				menu_administration();
				break;
			case 4:
				break;
			default:
				printf("Erreur\n");
				break;
		}
	}
}


// Menu des livres
void affichage_menu_livres() {
	print_titre_menu("Livres");
	char *menu =
		"[1]. Rechercher un livre\n"
		"[2]. Emprunter un livre\n"
		"[3]. Retourner un livre\n"
		"[4]. Livres empruntés\n"
		"[5]. Livres disponibles\n"
		"[6]. Retour\n";
	printf("%s", menu);
}

void menu_livres() {
	int choix = 0;
	while(choix != 6) {
		affichage_menu_livres();
		choix = selection(6);
		switch(choix) {
			case 1:
				rechercher_livre();
				saisie_entree();
				break;
			case 2:
				emprunter_livre();
				saisie_entree();
				break;
			case 3:
				retourner_livre();
				saisie_entree();
				break;
			case 4:
				livres_empruntes();
				saisie_entree();
				break;
			case 5:
				livres_disponibles();
				saisie_entree();
				break;
			case 6:
				break;
			default:
				printf("Erreur\n");
				break;
		}
	}
}


// Menu du compte
void affichage_menu_compte() {
	print_titre_menu("Compte");
	char *menu = 
		"[1]. Modifier le compte\n"
		"[2]. Supprimer le compte\n"
		"[3]. Afficher les informations du compte\n"
		"[4]. Retour\n";
	printf("%s", menu);
}

int menu_compte() {
	int choix = 0;
	while(choix != 4) {
		affichage_menu_compte();
		choix = selection(4);
		switch(choix) {
			case 1:
				modifier_compte();
				saisie_entree();
				break;
			case 2:
				if (supprimer_compte()) {
					return 1;
				}
				saisie_entree();
				break;
			case 3:
				afficher_compte();
				saisie_entree();
				break;
			case 4:
				break;
			default:
				printf("Erreur\n");
				break;
		}
	}
	return 0;
}


// Menu de l'administration
void affichage_menu_administration() {
	print_titre_menu("Administration");
	char *menu = 
		"[1]. Ajouter un livre\n"
		"[2]. Supprimer un livre\n"
		"[3]. Rechercher un compte\n"
		"[4]. Ajouter un compte\n"
		"[5]. Supprimer un compte\n"
		"[6]. Retour\n";
	printf("%s", menu);
}

void menu_administration() {
	int choix = 0;
	while(choix != 6) {
		affichage_menu_administration();
		choix = selection(6);
		switch(choix) {
			case 1:
				ajouter_livre();
				saisie_entree();
				break;
			case 2:
				supprimer_livre();
				saisie_entree();
				break;
			case 3:
				rechercher_compte();
				saisie_entree();
				break;
			case 4:
				ajouter_compte();
				saisie_entree();
				break;
			case 5:
				supprimer_compte_admin();
				saisie_entree();
				break;
			case 6:
				break;
			default:
				printf("Erreur\n");
				break;
		}
	}
}
