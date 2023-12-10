#include "structs.h"
#include "debug.h"

int init_Livre(Livre *livre, char *isbn, char *titre, char *auteur, char *genre, int id_user, char *date_emprunt) {
	if (isbn == NULL) isbn = "";
	if (titre == NULL) titre = "";
	if (auteur == NULL) auteur = "";
	if (genre == NULL) genre = "";
	if (date_emprunt == NULL) date_emprunt = "";

	strncpy(livre->isbn,isbn,14);
	strncpy(livre->titre,titre,100);
	strncpy(livre->auteur,auteur,100);
	strncpy(livre->genre,genre,100);
	livre->id_user = id_user;
	strncpy(livre->date_emprunt,date_emprunt,10);
	return 0;
}

void print_Livre(Livre *livre) {
	printf("%-10.10s %-10.10s %-10.10s %-10.10s %d %-10.10s\n", livre->isbn, livre->titre, livre->auteur, livre->genre, livre->id_user, livre->date_emprunt);
}

int init_Compte(Compte *compte, int id_user, char *nom, char *prenom, char *mail, int admin) {
	if (nom == NULL) nom = "";
	if (prenom == NULL) prenom = "";
	if (mail == NULL) mail = "";

	compte->id_user = id_user;
	strncpy(compte->nom,nom,100);
	strncpy(compte->prenom,prenom,100);
	strncpy(compte->mail,mail,100);
	compte->admin = admin;
	return 0;
}

void print_Compte(Compte *compte) {
	printf("%d  |%-10.10s|%-10.10s|%-10.10s|%d\n", compte->id_user, compte->nom, compte->prenom, compte->mail, compte->admin);
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
		bin = cel;
		cel = cel->suivant;
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

void afficher_listeLivre(listeLivre *L) {
	printf("ISBN\t\tTitre\t\tAuteur\t\tGenre\t\tEmprunté\tDate d'emprunt\n");
	celluleLivre *cel = L->tete;
	while (cel != NULL) {
		print_Livre(cel->livre);
		cel = cel->suivant;
	}
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

void afficher_listeCompte(listeCompte *L) {
	printf("ID\t\tNom\t\tPrénom\t\tMail\t\tAdmin\n");
	celluleCompte *cel = L->tete;
	while (cel != NULL) {
		print_Compte(&cel->compte);
		cel = cel->suivant;
	}
}
