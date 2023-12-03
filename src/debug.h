#ifndef __DEBUG__
#define __DEBUG__

#include <stdio.h>
#include "structs.h"

#ifdef DEBUG
#define debug(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...) ((void)0)
#endif

int err();
void print_Livre(Livre *livre);
void print_Compte(Compte *compte);

#endif 
