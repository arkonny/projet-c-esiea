#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>


int dispo(char *s);
void emprunt(Livre l);
void retour(Livre l);
int list_d();
void list_e();
void save();
int callback(void *NotUsed, int argc, char **argv, char **azColName);

#endif
