# Projet C
## Librairie numérique basée sur SQLite3
__autheurs__: Lucien Renard-Raguenaud & Anthony Uriel

## Lien du projet sur Github :
![Projet C](https://github.com/arkonny/projet-c-esiea)

## Fonctionnalités de Base :

1. Ajout de Livres
2. Recherche de Livres

    - possible de rechercher par mots-clés 

3. Emprunt et Retour de Livres

4. Affichage de la Liste des Livres

    - en faisant une recherche vide
5. Gestion des Livres Empruntés

    - affiche la liste des livres actuellement empruntés, avec la date d'emprunt et les détails de l'emprunteur (si le compte actuel est un compte admin, sinon pas de détails sur l'emprunteur, juste la date d'emprunt)

6. Sauvegarde des Données

    - via un fichier SQL
7. Interface Utilisateur

    - nous n'avons pas trouvé de librairie d'interface graphique utilisable sur windows en C. Utiliser Gtk+ aurais été trop complexe sur windows.
    Donc notre programme interagit uniquement par terminal.

## Bonus (pour les étudiants avancés) :
1. Gestion des Utilisateurs
    - les utilisateurs peuvent visualiser leurs informations
2. Recherche Avancée
    - Un livre peut être recherché par mot-clé, pas besoin de taper son titre ou n'importe quelle autre information en entier.
