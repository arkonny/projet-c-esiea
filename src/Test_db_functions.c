#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "db_functions.h"

sqlite3 *db;
Compte *currentUser;

// Main function tests each function of db_functions.c
int main() {
	int res;

	printf("\n=== Initialisation of Livre objects ===\n");
	Livre *book = malloc(sizeof(Livre));
	init_Livre(book, "xxxx","Book Title", "Author","Genre (mais en anglais)",1,""); 
	print_Livre(book);
	Livre *book2 = malloc(sizeof(Livre));
	init_Livre(book2, "xabx","Titre du livre", "Auteur","Genre",2,""); 
	print_Livre(book2);

	char *date = "2020-01-01";
	strcpy(book->date_emprunt, date);
	strcpy(book2->date_emprunt, date);

	Compte *user = malloc(sizeof(Compte));
	init_Compte(user, 0, "Renard-Raguenaud", "Lucien", "lucienrenardraguenaud@mail.com", 0);

	Compte *user2 = malloc(sizeof(Compte));
	init_Compte(user2, 2, "Uriel", "Anthony", "anthonyuriel@mail.com", 0);

	printf("\n=== Testing listeLivre functions ===\n");
	listeLivre *liste_empruntes = malloc(sizeof(listeLivre));
	init_listeLivre_vide(liste_empruntes);
	debug("liste_empruntes->taille = %d\n", liste_empruntes->taille);
	//ajouter_tete_listeLivre(liste_empruntes, book);

	listeLivre *result = malloc(sizeof(listeLivre));
	init_listeLivre_vide(result);
	debug("result->taille = %d\n", result->taille);

	res = SQL_open();
	printf("SQL_open() returned %d\n\n", res);
	if (res) {
		return err();
	}

	/*
	printf("\n=== Testing SQL_init ===\n");
	res = SQL_init();
	printf("SQL_init() returned %d\n\n", res);
	if (res) {
		return err();
	}
	*/

	/*
	printf("\n=== Testing SQL_ajout ===\n");
	res = SQL_ajout(book); // returns SQLITE_DONE
	printf("SQL_ajout(book) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	*/

	//*
	Compte *user_result0 = malloc(sizeof(Compte));
	init_Compte(user_result0, 0, "", "", "lr@gmail.com", 0);
	printf("\n=== Testing SQL_Compte_recherche===\n");
	res = SQL_Compte_recherche(user_result0);
	print_Compte(user_result0);
	printf("SQL_Compte_recherche(user_result) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	//*/
	
	///*
	printf("\n=== Testing SQL_creation_compte===\n");
	res = SQL_creation_compte(user, "password");
	print_Compte(user);
	printf("SQL_creation_compte(user2) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	//*/
	
	//*
	Compte *user_result = malloc(sizeof(Compte));
	init_Compte(user_result, 0, "", "", "lucienrenardraguenaud@mail.com", 0);
	printf("\n=== Testing SQL_Compte_recherche===\n");
	res = SQL_Compte_recherche(user_result);
	print_Compte(user_result);
	printf("SQL_Compte_recherche(user_result) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	//*/
	
	//*
	printf("\n=== Testing SQL_connexion===\n");
	res = SQL_connexion(user, "password");
	printf("SQL_connexion(user) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	//*/
	
	//*
	printf("\n=== Testing SQL_changement_mdp===\n");
	res = SQL_changement_mdp(user, "new_password");
	printf("SQL_changement_mdp(user) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	//*/
	
	//*
	printf("\n=== Testing SQL_changement_mail===\n");
	print_Compte(user);
	res = SQL_changement_mail(user, "new_mail");
	print_Compte(user);

	init_Compte(user_result, 0, "", "", "new_mail", 0);
	SQL_Compte_recherche(user_result);
	print_Compte(user_result);

	printf("SQL_changement_mail(user) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	//*/
	
	/*
	printf("\n=== Testing SQL_suppression_compte===\n");
	res = SQL_suppression_compte(user);
	printf("SQL_suppression_compte(user) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	*/

	/*
	printf("\n=== Testing SQL_emprunt ===\n");
	res = SQL_emprunt(book, user);
	printf("SQL_emprunt(book) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	*/


	/*
	printf("\n=== Testing SQL_disponibilite ===\n");
	res = SQL_disponibilite(book);
	printf("book->date_emprunt = %s\n", book->date_emprunt);
	printf("SQL_disponibilite(book) returned %d\n\n", res);
	if (res) {
		return err();
	}
	*/

	/*
	printf("\n=== Testing SQL_retour ===\n");
	res = SQL_retour(book);
	printf("SQL_retour(book) returned %d\n\n", res);
	if (res) {
		return err();
	}
	*/


	/*
	printf("\n=== Testing SQL_suppression ===\n");
	res = SQL_suppression(book);
	printf("SQL_suppression(book) returned %d\n\n", res);
	if (res) {
		return err();
	}
	*/
	

	//SQL_ajout(book2);

	//SQL_emprunt(book2, user);


	/*
	printf("\n=== Testing SQL_recherche ===\n");
	debug("Recherhe livre : ");
	print_Livre(book2);
	res = SQL_recherche(book2, result);
	printf("Il y a %d livre(s) correspondand(s) :\n",result->taille);
	celluleLivre* cel = result->tete;
	while (cel != NULL) {
		printf("livre: %s\n", cel->livre->titre);
		cel = cel->suivant;
	}
	printf("SQL_recherche(book) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	*/


	/*
	printf("\n=== Testing SQL_livres_empruntes ===\n");
	SQL_livres_empruntes(user, liste_empruntes);
	printf("Le compte %s a emprunte %d livres :\n", user.nom, liste_empruntes->taille);
	cel = liste_empruntes->tete;
	while (cel != NULL) {
		printf("livre: %s\n", cel->livre->titre);
		cel = cel->suivant;
	}
	printf("\nSQL_livres_empruntes(user, liste_empruntes) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	*/
	
	printf("\n=== Freeing objects ===\n");
	liberer_listeLivre(result);
	debug("result liberee\n");
	liberer_listeLivre(liste_empruntes);
	debug("liste_empruntes liberee\n");

	free(book);
	debug("book libere\n");
	free(book2);
	debug("book2 libere\n");
	free(user);
	debug("user libere\n");
	free(user2);
	debug("user2 libere\n");
	free(user_result);
	debug("user_result libere\n");
	free(user_result0);
	debug("user_result0 libere\n");


	res = SQL_close();
	printf("\nSQL_close() returned %d\n\n", res);
}
