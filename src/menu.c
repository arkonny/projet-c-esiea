#include "menu.h"

// Saisie d'un entier
// pour le choix d'un menu
int selection(int nbChoix) {
	int choix;
	printf("Votre choix : ");
	scanf("%d", &choix);
	while(choix < 1 || choix > nbChoix) {
		printf("Choix invalide\n");
		printf("Un nombre entre 1 et %d est attendu\n", nbChoix);
		printf("Votre choix : ");
		scanf("%d", &choix);
	}
	return choix;
}

// Menu principal
int affichage_menu_principal() {
	char *menu = 
		"=====================\n"
		"   Menu principal\n"
		"=====================\n\n"
		"[1]. Gestion des clients\n"
		"[2]. Gestion des comptes\n"
		"[3]. Gestion des opérations\n"
		"[4]. Quitter\n";
	printf("%s", menu);
	return 0;
}
