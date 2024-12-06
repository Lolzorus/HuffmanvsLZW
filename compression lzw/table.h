/* table.h - Fichier d'en-tête pour la définition de la table LZW */

struct EntreeLZW {
    unsigned char code_base;
    unsigned char caractere;
};

/* Macros pour lire et écrire des caractères */
#define LIRE_CHAR(x) fread(&(x), sizeof(unsigned char), 1, fichier_entree)
#define ECRIRE_CHAR(x) fwrite(&(x), sizeof(unsigned char), 1, fichier_sortie)

/* Prototypes de fonctions */
unsigned char extraire_chaine(unsigned char code);
int compresser_lzw(char *fichier_entree_nom, char *fichier_sortie_nom); // Prototype mis à jour
void decompresser_lzw(char *fichier_entree_nom, char *fichier_sortie_nom); // Prototype mis à jour

