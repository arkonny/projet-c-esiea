#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct Livre_s
    {
        char isbn[14];
        char titre[100];
        char auteur[100];
        char genre[100];
        int id_user;
        char date_emprunt[11];
    } Livre;

typedef struct Compte_s
    {
        int id_user;
        char nom[100];
        char prenom[100];
        char mail[100];
        char mdp[100];
        int droit;
    } Compte;

#endif
