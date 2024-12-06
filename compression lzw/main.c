/* main.c - Programme principal pour la compression LZW des fichiers texte */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include <time.h> // Pour benchmark du temps

void afficher_aide() {
    puts("Utilisation : lzw\n"
         "Appuyez sur 'c' pour compresser un fichier\n"
         "Appuyez sur 'd' pour décompresser un fichier\n"
         "Appuyez sur 'h' pour afficher ce message d'aide.");
    exit(EXIT_FAILURE);
}

int main() {
    char choix;
    char nom_fichier[256];
    char nom_fichier_sortie[256];

    printf("Bienvenue dans le programme de compression LZW !\n");
    printf("Entrez 'c' pour compresser, 'd' pour décompresser, ou 'h' pour l'aide : ");
    scanf(" %c", &choix); // Espace avant %c pour ignorer les espaces

    if (choix == 'h') {
        afficher_aide(); // Afficher l'aide si l'option 'h' est demandée
    }

    printf("Veuillez entrer le nom du fichier à traiter : ");
    scanf("%s", nom_fichier); // Lire le nom du fichier

    printf("Veuillez entrer le nom du fichier de sortie : ");
    scanf("%s", nom_fichier_sortie); // Lire le nom du fichier de sortie

    if (choix == 'c') {
    printf("Compression de %s en %s ... ", nom_fichier, nom_fichier_sortie);
    clock_t start = clock();
    int taux = compresser_lzw(nom_fichier, nom_fichier_sortie); // Passer les deux noms de fichiers
    clock_t end = clock();

    double timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Temps d'exécution : %.2f secondes\n", timeTaken);
    printf("(%d%%) terminé\n", taux);
	} else if (choix == 'd') {
	    printf("Décompression de %s en %s ... ", nom_fichier, nom_fichier_sortie);
	    clock_t start = clock();
	    decompresser_lzw(nom_fichier, nom_fichier_sortie); // Passer les deux noms de fichiers
	    clock_t end = clock();

	    double timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
	    printf("Temps d'exécution : %.2f secondes\n", timeTaken);
	    printf("terminé\n");
	}

    return 0;
}


