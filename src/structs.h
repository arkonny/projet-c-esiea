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

void print_Livre(Livre *livre);

typedef struct {
        int id_user;
        char nom[101];
        char prenom[101];
        char mail[101];
				char hash[101];
				int admin;
} Compte;

int init_Compte(Compte *compte, int id_user, char *nom, char *prenom, char *mail, char *hash, int admin);

void print_Compte(Compte *compte);

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
void afficher_listeLivre(listeLivre *L);


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
void afficher_listeCompte(listeCompte *L);

#endif
