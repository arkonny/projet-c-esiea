#############################################################################
# Fichier Makefile 
# UE MAP401 - DLST - UGA - 2020/2021
#############################################################################
# utilisation des variables internes $@ $< $^ $*
# $@ : correspond au nom de la cible
# $< : correspond au nom de la premiere dependance
# $^ : correspond � toutes les d�pendances
# $* : correspond au nom du fichier sans extension 
#       (dans les regles generiques uniquement)
#############################################################################
# information sur la regle executee avec la commande @echo
# (une commande commancant par @ n'est pas affichee a l'ecran)
#############################################################################


#############################################################################
# definition des variables locales
#############################################################################

# compilateur C
CC = gcc

# chemin d'acces aux librairies (interfaces)
INCDIR = .

# chemin d'acces aux librairies (binaires)
LIBDIR = .

# options pour l'addition des liens
LDOPTS = -lsqlite3 -lcrypt -L$(LIBDIR) -lm

# options pour la recherche des fichiers .o et .h
INCLUDEOPTS = -I$(INCDIR)

# options de debuggage si make debug
#DEBOPTS ?= -DDEBUG

# options de compilation
COMPILOPTS = -g $(DEBOPTS) -Wall -lsqlite3 -lcrypt $(INCLUDEOPTS)

# liste des executables
EXECUTABLES = main


#############################################################################
# definition des regles
#############################################################################

########################################################
# la règle par défaut
all : $(EXECUTABLES)

########################################################
# regle generique : 
#  remplace les regles de compilation separee de la forme
#	module.o : module.c module.h
#		$(CC) -c $(COMPILOPTS) module.c
%.o : %.c %.h debug.h structs.h db_functions.h menu.h menu_fonctions.h fonctions.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module "$*
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

########################################################
# regles explicites de compilation separee de modules
# n'ayant pas de fichier .h ET/OU dependant d'autres modules

db_functions.o : db_functions.c db_functions.h structs.h debug.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module db_functions"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

menu.o : menu.c menu.h menu_fonctions.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module menu"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

test%.o : test%.c db_functions.h structs.h debug.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module "$@
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

main.o : main.c db_functions.h structs.h debug.h menu.h fonctions.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module "$@
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

########################################################
# regles explicites de creation des executables

test% : test%.o db_functions.o debug.o structs.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -g -o $@

main : main.o db_functions.o debug.o structs.o menu.o fonctions.o menu_fonctions.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -g -o $@

# regle pour "nettoyer" le r�pertoire
clean:
	rm -fR $(EXECUTABLES) *.o *.eps

clear:
	rm -fR [^sqlite3]*.o *.eps
