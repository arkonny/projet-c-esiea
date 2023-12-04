#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        char isbn[15];
        char titre[101];
        char auteur[101];
        char genre[101];
        int id_user;
        char date_emprunt[11];
} Livre;

int init_Livre(Livre *livre, char *isbn, char *titre, char *auteur, char *genre, int id_user, char *date_emprunt);

typedef struct {
        int id_user;
        char nom[100];
        char prenom[100];
        char mail[100];
        char mdp[100];
        int droit;
} Compte;


typedef struct celluleLivre_s {
	Livre *livre;
	struct celluleLivre_s *suivant;
} celluleLivre;

typedef struct listeLivre_s{
	int taille;
	celluleLivre *tete;
} listeLivre;

void init_listeLivre_vide(listeLivre* L);
void liberer_listeLivre(listeLivre *L);
int ajouter_tete_listeLivre(listeLivre *L, Livre *livre);


typedef struct celluleCompte_s{
	Compte compte;
	struct celluleCompte_s *suivant;
} celluleCompte;

typedef struct listeCompte_s{
	int taille;
	celluleCompte *tete;
} listeCompte;

void init_listeCompte_vide(listeCompte* L);
void liberer_listeCompte(listeCompte *L);
int ajouter_tete_listeCompte(listeCompte *L, Compte compte);

#endif
