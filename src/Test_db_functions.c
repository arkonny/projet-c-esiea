#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "db_functions.h"

sqlite3 *db;

// Main function tests each function of db_functions.c
int main() {
	int res;
	
	//*
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

	Livre book = {"xxxx","Book Title", "Author","Genre",1,""}; 
	
	res = SQL_ajout(&book); // returns SQLITE_DONE
	printf("SQL_ajout(book) returned %d\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}
	printf("\n");


	Compte user = {1, "nom", "prenom", "mail", "mdp", 0};
	char *date = "2020-01-01";
	strcpy(book.date_emprunt, date);

	res = SQL_emprunt(&book, &user);
	printf("SQL_emprunt(book) returned %d\n\n", res);
	if (res) {
		return err();
	}

	res = SQL_disponibilite(&book);
	printf("book.date_emprunt = %s\n", book.date_emprunt);
	printf("SQL_disponibilite(book) returned %d\n\n", res);
	if (res) {
		return err();
	}

	
	res = SQL_retour(&book);
	printf("SQL_retour(book) returned %d\n\n", res);
	if (res) {
		return err();
	}

	
	res = SQL_suppression(&book);
	printf("SQL_suppression(book) returned %d\n\n", res);
	if (res) {
		return err();
	}
	
	
	Livre book2 = {"yyyy","Titre du livre", "Auteur","Genre",0,""}; 

	res = SQL_ajout(&book2);
	printf("SQL_ajout(book) returned %d\n\n", res);
	if (res != SQLITE_DONE) {
		return err();
	}

	Compte user2 = {1, "nom", "prenom", "mail", "mdp", 0};
	strcpy(book2.date_emprunt, date);

	res = SQL_emprunt(&book2, &user2);
	printf("SQL_emprunt(book) returned %d\n\n", res);
	if (res) {
		return err();
	}

	res = SQL_recherche(&book2);

	res = SQL_close();
	printf("SQL_close() returned %d\n\n", res);
}
