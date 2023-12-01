#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
//#include "structs.h"
#include "db_functions.h"

sqlite3 *db;

// Main function tests each function of db_functions.c
int main() {
	int res;

	
	//*
	res = SQL_init();
	printf("SQL_init() returned %d\n", res);
	if (res) {
		printf("SQL_init() fail\n");
		return 1;
	}
	//*/

	/*
	res = SQL_open();
	printf("SQL_open() returned %d\n", res);
	if (res) {
		printf("SQL_open() fail\n");
		return 1;
	}
	*/

	Livre book = {"xxxx","Book Title", "Author","Genre",1,1,""}; 
	
	res = SQL_ajout(&book);
	printf("SQL_ajout(book) returned %d\n", res);
	if (res) {
		printf("SQL_ajout(book) fail\n");
		return 1;
	}

	Compte user = {1, "nom", "prenom", "mail", "mdp", 0};

	res = SQL_emprunt(&book, &user);
	printf("SQL_emprunt(book) returned %d\n", res);
	if (res) {
		printf("SQL_emprunt(book) fail\n");
		return 1;
	}

	res = SQL_close();
	printf("SQL_close() returned %d\n", res);
	if (res) {
		printf("SQL_close() fail\n");
		return 1;
	}
}
