#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "db_functions.h"

sqlite3 *db;
Compte current_user;

// Main function tests each function of db_functions.c
int main() {
	int res;

	printf("\n=== Initialisation of Livre objects ===\n");
	Livre *book = malloc(sizeof(Livre));
	init_Livre(book, "xxxx","Book Title", "Author","Genre",1,""); 
	print_Livre(book);
	Livre *book2 = malloc(sizeof(Livre));
	init_Livre(book2, "yyyy","Titre du livre", "Auteur","Genre",0,""); 
	print_Livre(book2);

	char *date = "2020-01-01";
	strcpy(book->date_emprunt, date);
	strcpy(book2->date_emprunt, date);

	Compte user = {1, "Renard-Raguenaud", "Lucien", "lucienrenardraguenaud@mail.com", "mot-de-passe", 0};

	printf("\n=== Testing listeLivre functions ===\n");
	listeLivre *liste_empruntes = malloc(sizeof(listeLivre));
	init_listeLivre_vide(liste_empruntes);
	debug("liste_empruntes->taille = %d\n", liste_empruntes->taille);
	//ajouter_tete_listeLivre(liste_empruntes, book);

	listeLivre *result = malloc(sizeof(listeLivre));
	init_listeLivre_vide(result);
	debug("result->taille = %d\n", result->taille);

	
	///*
	printf("\n=== Testing SQL_init ===\n");

	res = SQL_init();
	printf("SQL_init() returned %d\n\n", res);
	if (res) {
		return err();
	}
	//*/

	/*
	res = SQL_open();
	printf("SQL_open() returned %d\n\n", res);
	if (res) {
		return err();
	}
	*/

	///*
	printf("\n=== Testing SQL_ajout ===\n");
	res = SQL_ajout(book); // returns SQLITE_DONE
	printf("SQL_ajout(book) returned %d\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	printf("\n");


	printf("\n=== Testing SQL_emprunt ===\n");
	res = SQL_emprunt(book, &user);
	printf("SQL_emprunt(book) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}


	printf("\n=== Testing SQL_disponibilite ===\n");

	res = SQL_disponibilite(book);
	printf("book->date_emprunt = %s\n", book->date_emprunt);
	printf("SQL_disponibilite(book) returned %d\n\n", res);
	if (res) {
		return err();
	}

	/*
	printf("\n=== Testing SQL_retour ===\n");
	
	res = SQL_retour(book);
	printf("SQL_retour(book) returned %d\n\n", res);
	if (res) {
		return err();
	}


	printf("\n=== Testing SQL_suppression ===\n");
	
	res = SQL_suppression(book);
	printf("SQL_suppression(book) returned %d\n\n", res);
	if (res) {
		return err();
	}
	*/
	

	SQL_ajout(book2);

	SQL_emprunt(book2, &user);


	/*
	printf("\n=== Testing SQL_recherche ===\n");
	res = SQL_recherche(book2, result);
	printf("SQL_recherche(book) returned %d\n\n", res);
	if (res) {
		return err();
	}
	*/


	///*
	printf("\n=== Testing SQL_livres_empruntes ===\n");
	SQL_livres_empruntes(&user, liste_empruntes);
	printf("Le compte %s a emprunte %d livres :\n", user.nom, liste_empruntes->taille);
	celluleLivre* cel = liste_empruntes->tete;
	while (cel != NULL) {
		printf("livre: %s\n", cel->livre->titre);
		cel = cel->suivant;
	}
	printf("\nSQL_livres_empruntes(user, liste_empruntes) returned %d\n\n", res);
	if (res) {
		return err();
	}
	//*/

	printf("\n=== Freeing objects ===\n");
	debug("liberer result\n");
	liberer_listeLivre(result);
	debug("result liberee\n");
	debug("\nliberer liste_empruntes\n");
	liberer_listeLivre(liste_empruntes);
	debug("liste_empruntes liberee\n");
	res = SQL_close();
	printf("\nSQL_close() returned %d\n\n", res);
}
