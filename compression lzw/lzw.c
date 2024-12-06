#include <stdlib.h>   
#include <stdio.h>    
#include <fcntl.h>    // Pour les opérations sur les fichiers
#include <string.h>   // Pour les fonctions de manipulation de chaînes
#include "table.h"    // Pour inclure la définition de la structure de la table LZW

// Déclaration des pointeurs de fichiers pour l'entrée et la sortie
FILE *fichier_entree, *fichier_sortie;

// Variable globale pour indiquer si la table LZW est pleine
int table_complete = 1;

// Structure représentant une entrée de la table LZW
struct EntreeLZW table_lzw[256];


/**
 * Fonction : initialiser_table
 * Description : Initialise la table LZW avec les caractères ASCII de 0 à 127 et des entrées vides après.
 * Paramètres : Aucun.
 */

void initialiser_table() {
    // Remplit la table avec les caractères ASCII de 0 à 127
    for (int i = 0; i < 128; i++) {
        table_lzw[i].code_base = table_lzw[i].caractere = (unsigned char)i; // Code et caractère initialisés à i
    }
    // Remplit le reste de la table avec des caractères nuls
    for (int i = 128; i < 256; i++) {
        table_lzw[i].code_base = table_lzw[i].caractere = '\0'; // Indique une entrée vide
    }
}

// On a notre table avec les caractere ASCII et des cases vide (limite les risques de chevauchement etc etc)



/**
 * Fonction : rechercher_entree
 * Description : Recherche une entrée dans la table LZW.
 * Paramètres : 
 * - caractere : Le caractère à rechercher.
 * - code_base : Le code de base à vérifier.
 * - index_table : Pointeur vers la variable qui recevra l'index trouvé ou vide.
 * Retourne : 1 si trouvé, 0 sinon.
 */

int rechercher_entree(unsigned char caractere, unsigned char code_base, int *index_table) {
    // Parcourt la table à partir de l'index 128
    for (int i = 128; i < 256; i++) {
        // Vérifie si l'entrée correspond au code de base et au caractère
        if (table_lzw[i].code_base == code_base && table_lzw[i].caractere == caractere) {
            *index_table = i; // Renvoie l'index de l'entrée trouvée
            return 1; // Trouvé
        }
        // Vérifie si l'entrée est vide
        if (table_lzw[i].caractere == '\0') {
            *index_table = i; // Renvoie l'index de la première entrée vide
            return 0; // Non trouvé
        }
    }
    // Si aucune entrée n'est trouvée, la table est pleine
    table_complete = 0; // La table est pleine
    *index_table = (int)caractere; // Renvoie l'index du caractère
    return 0; // Non trouvé
}

// on a une table et une fonction pour rechercher les caractere et les verifier si les caracteres correspondent

/**
 * Fonction : ajouter_code
 * Description : Ajoute un nouveau code dans la table LZW.
 * Paramètres :
 * - caractere : Le caractère à ajouter.
 * - code_base : Le code de base associé.
 * - index_table : L'index où l'entrée doit être ajoutée.
 */

void ajouter_code(unsigned char caractere, unsigned char code_base, int index_table) {
    table_lzw[index_table].code_base = code_base; // Met à jour le code de base
    table_lzw[index_table].caractere = caractere; // Met à jour le caractère
}

/**
 * Fonction : code_absent_dans_table
 * Description : Vérifie si un code est absent dans la table LZW.
 * Paramètres :
 * - code : Le code à vérifier.
 * Retourne : 1 si le code est absent, 0 sinon.
 */

int code_absent_dans_table(unsigned char code) {
    return table_lzw[code].caractere == '\0'; // Retourne vrai si l'entrée est vide
}


/**
 * Fonction : extraire_chaine
 * Description : Reconstruit une chaîne à partir d'un code et l'écrit dans le fichier de sortie.
 * Paramètres :
 * - code : Le code à extraire.
 * Retourne : Le dernier caractère extrait.
 */
unsigned char extraire_chaine(unsigned char code) {
    unsigned char chaine_temp[80] = {0}; // Tableau temporaire pour stocker la chaîne
    static unsigned char caractere_temp; // Variable pour stocker le caractère temporaire

    // Si le code est inférieur à 128, c'est un caractère de base
    if (code < 128) {
        ECRIRE_CHAR(code); // Écrire le caractère dans le fichier de sortie
        return code; // Retourner le code
    }

    int i = 0; // Index pour la chaîne temporaire
    // Boucle pour reconstruire la chaîne à partir du code (faire une recherche dans la table > ASCII 127)
    while (code > 127) {
        caractere_temp = table_lzw[code].caractere; // Obtenir le caractère associé
        chaine_temp[i++] = caractere_temp; // Ajouter le caractère à la chaîne
        code = table_lzw[code].code_base; // Mettre à jour le code
    }
    chaine_temp[i++] = code; // Ajouter le dernier caractère

    // Écrire les caractères dans l'ordre inverse
    for (int j = i - 1; j >= 0; j--) {
        ECRIRE_CHAR(chaine_temp[j]); // Écrire chaque caractère dans le fichier
    }
    return chaine_temp[i - 1]; // Retourner le dernier caractère
}


/**
 * Fonction : compresser_lzw
 * Description : Compresse un fichier avec l'algorithme LZW.
 * Paramètres :
 * - fichier_entree_nom : Nom du fichier d'entrée à compresser.
 * - fichier_sortie_nom : Nom du fichier de sortie où la compression est écrite.
 * Retourne : Le taux de compression en pourcentage.
 */
int compresser_lzw(char *fichier_entree_nom, char *fichier_sortie_nom) {
    unsigned char code_base, caractere_lu; // Variables pour stocker le code de base et le caractère lu
    int index; // Variable pour stocker l'index de la table
    long int compte_entrees = 0L, compte_sorties = 0L; // Compteurs pour les entrées et sorties

    // Ouverture du fichier d'entrée
    fichier_entree = fopen(fichier_entree_nom, "r");
    if (!fichier_entree) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", fichier_entree_nom); // Message d'erreur
        exit(EXIT_FAILURE); // Sortie en cas d'erreur
    }

    // Ouverture du fichier de sortie
    fichier_sortie = fopen(fichier_sortie_nom, "w+");
    if (!fichier_sortie) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", fichier_sortie_nom); // Message d'erreur
        fclose(fichier_entree); // Fermer le fichier d'entrée
        exit(EXIT_FAILURE); // Sortie en cas d'erreur
    }

    initialiser_table(); // Initialiser la table LZW
    LIRE_CHAR(code_base); // Lire le premier caractère
    compte_entrees++; // Incrémenter le compteur d'entrées

    // Boucle pour lire les caractères et compresser
    while (LIRE_CHAR(caractere_lu)) {
        compte_entrees++; // Incrémenter le compteur d'entrées
        if (rechercher_entree(caractere_lu, code_base, &index)) {
            code_base = (unsigned char)index; // Mettre à jour le code de base
        } else {
            ECRIRE_CHAR(code_base); // Écrire le code de base dans le fichier de sortie
            compte_sorties++; // Incrémenter le compteur de sorties
            if (table_complete) {
                ajouter_code(caractere_lu, code_base, index); // Ajouter le code à la table
            }
            code_base = caractere_lu; // Mettre à jour le code de base avec le caractère lu
        }
    }
    ECRIRE_CHAR(code_base); // Écrire le dernier code de base
    compte_sorties++; // Incrémenter le compteur de sorties

    // Fermer les fichiers
    fclose(fichier_sortie);
    fclose(fichier_entree);

    // Résumé de la compression
    printf("Résumé de la compression :\n");
    printf("Total d'entrées : %ld\n", compte_entrees); // Afficher le total d'entrées
    printf("Total de sorties : %ld\n", compte_sorties); // Afficher le total de sorties
    printf("Taux de compression : %d%%\n", (int)(((float)compte_sorties / (float)compte_entrees) * 100.0)); // Afficher le taux de compression

    return (int)(((float)compte_sorties / (float)compte_entrees) * 100.0); // Retourner le taux de compression
}


/**
 * Fonction : decompresser_lzw
 * Description : Décompresse un fichier avec l'algorithme LZW.
 * Paramètres :
 * - fichier_entree_nom : Nom du fichier d'entrée à décompresser.
 * - fichier_sortie_nom : Nom du fichier de sortie où la décompression est écrite.
 * Retourne : Aucun.
 */
void decompresser_lzw(char *fichier_entree_nom, char *fichier_sortie_nom) {
    unsigned char code, dernier_code, dernier_caractere; // Variables pour stocker les codes et caractères
    int prochain_code = 128; // Prochain code à ajouter à la table

    // Ouverture du fichier d'entrée
    fichier_entree = fopen(fichier_entree_nom, "r");
    if (!fichier_entree) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", fichier_entree_nom); // Message d'erreur
        exit(EXIT_FAILURE); // Sortie en cas d'erreur
    }

    // Ouverture du fichier de sortie
    fichier_sortie = fopen(fichier_sortie_nom, "w+");
    if (!fichier_sortie) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", fichier_sortie_nom); // Message d'erreur
        fclose(fichier_entree); // Fermer le fichier d'entrée
        exit(EXIT_FAILURE); // Sortie en cas d'erreur
    }

    initialiser_table(); // Initialiser la table LZW
    LIRE_CHAR(code); // Lire le premier code
    ECRIRE_CHAR(code); // Écrire le premier code dans le fichier de sortie
    dernier_code = code; // Stocker le dernier code

    int total_codes = 1; // Compteur pour les codes traités

    // Boucle pour lire les codes et décompresser
    while (LIRE_CHAR(code)) {
        if (code_absent_dans_table(code)) {
            dernier_caractere = extraire_chaine(dernier_code); // Extraire la chaîne du dernier code
            ECRIRE_CHAR(dernier_caractere); // Écrire le caractère dans le fichier de sortie
        } else {
            dernier_caractere = extraire_chaine(code); // Extraire la chaîne du code actuel
        }
        // Ajouter le nouveau code à la table si elle n'est pas pleine
        if (table_complete) {
            ajouter_code(dernier_caractere, dernier_code, prochain_code); // Ajouter le code à la table
            prochain_code++; // Incrémenter le prochain code
            if (prochain_code > 255) {
                table_complete = 0; // La table est pleine
            }
        }
        dernier_code = code; // Mettre à jour le dernier code
        total_codes++; // Incrémenter le compteur de codes traités
    }

    // Fermer les fichiers
    fclose(fichier_sortie);
    fclose(fichier_entree);

    // Résumé de la décompression
    printf("Résumé de la décompression :\n");
    printf("Total de codes traités : %d\n", total_codes); // Afficher le total de codes traités
}

