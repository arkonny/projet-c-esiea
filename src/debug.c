#include "debug.h"
#include "db_functions.h"

int err() {
	printf("Last result was not expected\n\n");
	SQL_close();
	return 1;
}

void print_Livre(Livre *livre) {
	debug("Livre: %s, %s, %s, %s, %d, %s\n", livre->isbn, livre->titre, livre->auteur, livre->genre, livre->id_user, livre->date_emprunt);
}

void print_Compte(Compte *compte) {
	debug("Compte: %d, %s, %s, %s, %s, %d\n", compte->id_user, compte->nom, compte->prenom, compte->mail, compte->mdp, compte->droit);
}
