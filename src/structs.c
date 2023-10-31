#include <stdio.h>
#include "all.h"

struct Livre
    {
        int isbn;
        char titre[100];
        char auteur[100];
        char genre[100];
        int id_user;
        int disponibilite;
        char date_e[11];
    };

struct Compte
    {
        int id_user;
        char nom[100];
        char prenom[100];
        char mail[100];
        char mdp[100];
        char droit[10];
    };

