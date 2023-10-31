#############################################################################
# Fichier Makefile 
# UE MAP401 - DLST - UGA - 2020/2021
#############################################################################
# utilisation des variables internes $@ $< $^ $*
# $@ : correspond au nom de la cible
# $< : correspond au nom de la premiere dependance
# $^ : correspond a toutes les dependances
# $* : correspond au nom du fichier sans extension 
#       (dans les regles generiques uniquement)
#############################################################################
# information sur la regle executee avec la commande @echo
# (une commande commencant par @ n'est pas affichee a l'ecran)
#############################################################################


#############################################################################
# definition des variables locales
#############################################################################

# compilateur C
CC = gcc
# chemin d'acces aux librairies (binaires)
LIBDIR = sqlite3

# options de compilation
COMPILOPTS = -g -Wall -Wextra

# liste des executables
EXECUTABLES = sqlite_learn

# Dossiers :
# - src : contient les fichiers sources
SRC = src
# - bin : contient les executables
BIN = bin

#############################################################################
# definition des regles
#############################################################################

########################################################
# la regle par defaut
all : $(EXECUTABLES)

########################################################
# regle generique : 
#  remplace les regles de compilation separee de la forme
#	module.o : module.c module.h
#		$(CC) -c $(COMPILOPTS) module.c
%.o : %.c %.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module "$*
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

sqlite_learn: $(SRC)/sqlite_learn.c
	$(CC) -o $(BIN)/$@ $< $(COMPILOPTS) -l$(LIBDIR)

# regle pour "nettoyer" le repertoire
clean:
	rm -fR $(SRC)/$(EXECUTABLES) *.o *.eps
