#ifndef ALL_H
#define ALL_H
#include <sqlite3.h>
#include "structs.h"

int dispo(char *s);
void emprunt(Livre l);
void retour(Livre l);
int list_d();
void list_e();
void save();
int callback(void *, int, char **, char **);

#endif