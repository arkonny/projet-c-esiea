#include "structs.h"
#include "debug.h"

int init_Livre(Livre *livre, char *isbn, char *titre, char *auteur, char *genre, int id_user, char *date_emprunt) {
	livre->isbn = (const unsigned char *) isbn;
	livre->titre = (const unsigned char *) titre;
	livre->auteur = (const unsigned char *) auteur;
	livre->genre = (const unsigned char *) genre;
	livre->id_user = id_user;
	livre->date_emprunt = (const unsigned char *) date_emprunt;
	return 0;
}

void liberer_Livre(Livre *livre) {
	free((void *) livre->isbn);
	free((void *) livre->titre);
	free((void *) livre->auteur);
	free((void *) livre->genre);
	free((void *) livre->date_emprunt);
}

void init_listeLivre_vide(listeLivre* L) {
	L->tete = NULL;
	L->taille = 0;
}

void liberer_listeLivre(listeLivre* L) {
	celluleLivre* cel = L->tete;
	celluleLivre* bin;
	int i = 0;
	while (cel != NULL) {
		debug("liberer_listeLivre: i = %d\n", i);
		bin = cel;
		cel = cel->suivant;
		liberer_Livre(bin->livre);
		free(bin);
		i++;
	}
	debug("liberer_listeLivre: i total = %d\n", i);
	L->tete = NULL;
	L->taille = 0;
}

int ajouter_tete_listeLivre(listeLivre* L, Livre *s) { /* retourne 0 si OK, 1 sinon  */
	celluleLivre* cel = (celluleLivre*) malloc(sizeof(celluleLivre));
	if (cel != NULL) {
		cel->livre = s;
		cel->suivant = L->tete;
		L->tete = cel;
		L->taille++;
		return 0;
	}
	return 1;
}

void init_listeCompte_vide(listeCompte* L) {
	L->tete = NULL;
	L->taille = 0;
}

void liberer_listeCompte(listeCompte* L) {
	celluleCompte* cel = L->tete;
	celluleCompte* bin;
	while (cel != NULL) {
		bin = cel;
		cel = cel->suivant;
		free(bin);
	}
	L->tete = NULL;
	L->taille = 0;
}

int ajouter_tete_listeCompte(listeCompte* L, Compte s) { /* retourne 0 si OK, 1 sinon  */
	celluleCompte* cel = (celluleCompte*) malloc(sizeof(celluleCompte));
	if (cel != NULL) {
		cel->compte = s;
		cel->suivant = L->tete;
		L->tete = cel;
		L->taille++;
		return 0;
	}
	return 1;
}
