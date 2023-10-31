#include <stdio.h>

int compterOccurencesA(char* chaine) {
    int occurences = 0;

    while (*chaine != '\0') {
        if (*chaine == 'A' || *chaine == 'a') {
            occurences++;
        }
        chaine++;
    }

    return occurences;
}

int main() {
    char chaine[] = "Alphabet avec des A et des a";

    int resultat = compterOccurencesA(chaine);

    printf("Le nombre d'occurences de 'A' est : %d\n", resultat);

    return 0;
}