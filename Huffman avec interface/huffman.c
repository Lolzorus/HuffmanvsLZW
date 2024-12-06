#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h> // pour les stats de compression

/**
 * Struct : MinHeapNode* newNode
 * Description : Crée un nouveau nœud pour l'arbre de Huffman, avec un caractère et une fréquence spécifiés.
 *               Ce nœud peut être utilisé dans un tas binaire (min-heap) pour construire l'arbre de Huffman.
 * Paramètres :
 * - char data : Le caractère associé au nœud. Il représente un caractère de l'entrée à encoder.
 * - unsigned freq : La fréquence d'apparition de ce caractère. Cette valeur est utilisée pour construire l'arbre de Huffman.
 * Retour :
 * - struct MinHeapNode* : Un pointeur vers le nouveau nœud alloué dynamiquement.
 *                         Si l'allocation échoue, la fonction retourne NULL.
 */
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* node = malloc(sizeof(struct MinHeapNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}



/**
 * Fonction : swap
 * Description : Échange deux nœuds dans le tas pour maintenir la propriété de l'arbre binaire
 * Paramètres :
 * - MinHeapNode** a: pointeur vers premier noeud a echanger
 * - MinHeapNode** a : pointeur vers deuxieme noeud a echanger
 */
void swap(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Fonction : minHeapify
 * Description : Réorganise un tas binaire (min-heap) pour maintenir la propriété du tas 
 *               après une modification. Cela garantit que chaque parent est plus petit 
 *               que ses enfants dans le sous-arbre correspondant.
 * Paramètres :
 * - struct MinHeapNode* minHeap[] : Tableau représentant le tas binaire.
 * - int size : Taille actuelle du tas.
 * - int idx : Indice du nœud à réorganiser.
 * Remarque : 
 * - Cette fonction est appelée récursivement pour descendre dans l'arbre et corriger
 *   les violations de la propriété du tas.
 */
void minHeapify(struct MinHeapNode* minHeap[], int size, int idx) {
    int smallest = idx; // le nœud courant est le plus petit
    int left = 2 * idx + 1; // Indice gauche.
    int right = 2 * idx + 2; // Indice droit.

     // Vérifie si gauche est plus petit que le nœud courant.
    if (left < size && minHeap[left]->freq < minHeap[smallest]->freq)
        smallest = left;

     // Vérifie si droit est plus petit que le plus petit trouvé jusqu'à présent.
    if (right < size && minHeap[right]->freq < minHeap[smallest]->freq)
        smallest = right;

     // Si le plus petit n'est pas le nœud courant, échange et continue la réorganisation.
    if (smallest != idx) {
        swap(&minHeap[smallest], &minHeap[idx]);
        minHeapify(minHeap, size, smallest);
    }
}
// On réorganise le tas pour que l'élément à l'indice donné respecte la propriété de min-heap



/**
 * Fonction : extractMin
 * Description : Extrait le nœud ayant la plus petite fréquence (racine du tas binaire),
 *               puis réorganise le tas pour maintenir la propriété du min-heap.
 * Paramètres :
 * - struct MinHeapNode* minHeap[] : Tableau représentant le tas binaire (min-heap).
 * - int* size : Pointeur vers la taille actuelle du tas. La taille est réduite après l'extraction.
 * Retour :
 * - struct MinHeapNode* : Le nœud extrait, qui contient la plus petite fréquence.
 *                         Après l'extraction, ce nœud est retiré du tas.
 */
struct MinHeapNode* extractMin(struct MinHeapNode* minHeap[], int* size) {
    // Enregistrer le nœud à extraire (le plus petit nœud, à la racine du tas)
    struct MinHeapNode* temp = minHeap[0];
    minHeap[0] = minHeap[--(*size)]; // Remplace la racine par le dernier nœud du tas
    minHeapify(minHeap, *size, 0); // Réorganise le tas pour restaurer la propriété du min-heap
    return temp; // Retourne le nœud extrait
}

/**
 * Fonction : insertMinHeap
 * Description : Insère un nouveau nœud dans un tas binaire (min-heap) tout en maintenant 
 *               la propriété de min-heap. Cela garantit que chaque parent reste plus petit 
 *               que ses enfants après l'ajout d'un nouvel élément.
 * Paramètres :
 * - struct MinHeapNode* minHeap[] : Tableau représentant le tas binaire.
 * - int* size : Pointeur vers la taille actuelle du tas (sera incrémentée).
 * - struct MinHeapNode* node : Nœud à insérer dans le tas.
 * Détails :
 * - Le nœud est d'abord ajouté à la fin du tas.
 * - Ensuite, il est remonté (heapify-up) dans le tas jusqu'à ce que la propriété de min-heap soit rétablie.
 */

void insertMinHeap(struct MinHeapNode* minHeap[], int* size, struct MinHeapNode* node) {
    minHeap[(*size)++] = node; // Ajoute le nouveau nœud à la fin du tas
    int i = *size - 1;

    // Remonte le nœud tant que la propriété de min-heap n'est pas respectee
    while (i && minHeap[i]->freq < minHeap[(i - 1) / 2]->freq) {
        // Échange le nœud avec son parent si nécessaire
        swap(&minHeap[i], &minHeap[(i - 1) / 2]);
        i = (i - 1) / 2; // Passe au parent
    }
}


/**
 * Fonction : buildHuffmanTree
 * Description : Construit l'arbre de Huffman à partir d'un tableau de fréquences de caractères 
 *               et génère les codes binaires correspondants pour chaque caractère.
 * Paramètres :
 * - int freq[] : Tableau contenant les fréquences des caractères (indexé par leur valeur ASCII).
 * - char codes[MAX_CHAR][MAX_CHAR] : Tableau pour stocker les codes binaires générés pour chaque caractère.
 * Étapes :
 * 1. Crée un nœud pour chaque caractère ayant une fréquence non nulle et l'insère dans un min-heap.
 * 2. Fusionne les deux nœuds ayant les plus petites fréquences jusqu'à ce qu'un seul nœud reste (racine de l'arbre de Huffman).
 * 3. Parcourt l'arbre de Huffman pour générer les codes binaires pour chaque caractère.
 */

void buildHuffmanTree(int freq[], char codes[MAX_CHAR][MAX_CHAR]) {
    struct MinHeapNode* minHeap[MAX_CHAR];
    int size = 0;

    // Étape 1 : Création des nœuds pour les caractères ayant une fréquence non nulle
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i]) {
            insertMinHeap(minHeap, &size, newNode((char)i, freq[i]));
        }
    }

    // Étape 2 : Construction de l'arbre de Huffman
    while (size > 1) {
        // Extraire les deux nœuds ayant les fréquences minimales
        struct MinHeapNode* left = extractMin(minHeap, &size);
        struct MinHeapNode* right = extractMin(minHeap, &size);
        // Crée un nouveau nœud avec la somme des fréquences des deux nœuds extraits
        struct MinHeapNode* top = newNode('$', left->freq + right->freq); 
        top->left = left;
        top->right = right;
        // Insère le nouveau nœud dans le min-heap
        insertMinHeap(minHeap, &size, top);
    }

     // Étape 3 : Génération des codes binaires à partir de l'arbre de Huffman
    char temp[MAX_CHAR]; // Tableau temporaire pour stocker le chemin actuel
    int top = 0; // Indice pour gérer le tableau temporaire
    struct MinHeapNode* root = minHeap[0]; // Racine de l'arbre de Huffman

    // Fonction auxiliaire pour parcourir l'arbre et stocker les codes
    void storeCodes(struct MinHeapNode* node) {
        if (!node) return;

        // Ajouter '0' au chemin actuel pour descendre à gauche
        if (node->left) {
            temp[top++] = '0';
            storeCodes(node->left);
            top--; // backtrack
        }

        // Ajouter '1' au chemin actuel pour descendre à droite
        if (node->right) {
            temp[top++] = '1';
            storeCodes(node->right);
            top--; // backtrack
        }

        // Si un nœud feuille est atteint, le code est complet
        if (!node->left && !node->right) {
            temp[top] = '\0'; // Terminer la chaîne de caractères
            strcpy(codes[(unsigned char)node->data], temp); // Stocker le code dans le tableau
        }
    }

    // Lancer le parcours de l'arbre à partir de la racine
    storeCodes(root);
}

// a ce stade nous avons nos structure de noeud et nos fonctions pour les manipuler. En creer, en rajouter a la pile, changer leurs positions.
// Nous avons aussi la fonction pour creer l'arbre et generer les codes correspondants

/**
 * Fonction : compressFile
 * Description : Compresse un fichier texte en utilisant l'algorithme de Huffman. 
 *               Le fichier compressé contient la table des fréquences suivie des données compressées.
 * Paramètres :
 * - const char* inputFile : Nom du fichier d'entrée à compresser.
 * - const char* outputFile : Nom du fichier de sortie pour stocker les données compressées.
 */
void compressFile(const char* inputFile, const char* outputFile) {
    FILE *inFile = fopen(inputFile, "r");  // Ouverture du fichier d'entrée en mode lecture
    if (!inFile) {
        perror("Ne peut pas ouvrir le fichier");
        return;
    }

    int freq[MAX_CHAR] = {0};
    char caractere_lu;

    // Étape 1 : Analyser le fichier pour calculer la fréquence d'apparition de chaque caractère
    while ((caractere_lu = fgetc(inFile)) != EOF) {
        freq[(unsigned char)caractere_lu]++;
    }
    fclose(inFile);

    // Print de debug
    printf("Frequence caracteres:\n");
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i] > 0) {
            printf("Character: '%c' Frequence: %d\n", (char)i, freq[i]);
        }
    }

    // Étape 2 : Construire l'arbre de Huffman à partir des fréquences et générer les codes binaires pour chaque caractère
    char codes[MAX_CHAR][MAX_CHAR] = {0};
    buildHuffmanTree(freq, codes);

    // Print des codes Huffman pour debug
    printf("Codes Huffman:\n");
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i] > 0) {
            printf("Character: '%c' Code: %s\n", (char)i, codes[i]);
        }
    }

    // Ouverture du fichier de sortie en mode binaire
    FILE *outFile = fopen(outputFile, "wb");
    if (!outFile) {
        perror("Ne peut pas ouvrir le fichier");
        return;
    }


    // Écriture de la table des fréquences dans le fichier de sortie
    fwrite(freq, sizeof(int), MAX_CHAR, outFile);

    // Réouverture du fichier d'entrée pour encoder son contenu
    inFile = fopen(inputFile, "r");
    unsigned char buffer = 0; // Buffer pour stocker les bits
    int bitCount = 0; // Compte le nombre de bits dans le buffer
    unsigned long totalBitsWritten = 0; // Suit le total bits ecrits

    // Étape 3 : Encoder le contenu du fichier en utilisant les codes de Huffman et écrire les bits compressés dans le fichier de sortie
    //Encodage en utilisant le buffer
    while ((caractere_lu = fgetc(inFile)) != EOF) {
        char* code = codes[(unsigned char)caractere_lu];
        for (int i = 0; code[i]; i++) {
            buffer = (buffer << 1) | (code[i] - '0'); // Ajouter un bit au buffer
            bitCount++;
            totalBitsWritten++;
            if (bitCount == 8) { // Si le buffer est plein, écrire un octet
                fputc(buffer, outFile);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Finaliser l'écriture du fichier compressé en ajoutant les bits restants dans le buffer
    // Gestion des bits restant pour eviter certains problemes en fin de fichier
    if (bitCount > 0) {
        buffer <<= (8 - bitCount); // Compléter avec des zéros
        fputc(buffer, outFile);
    }


    // print de debug pour le benchmark
    long originalSize = getFileSize(inputFile);
    long compressedSize = getFileSize(outputFile);
    if (originalSize > 0 && compressedSize > 0) {
        double compressionRatio = (double)originalSize / compressedSize;
        printf("Taille originale : %ld octets\n", originalSize);
        printf("Taille compressée : %ld octets\n", compressedSize);
        printf("Taux de compression : %.2f\n", compressionRatio);
    }

    fclose(inFile);
    fclose(outFile);


    // Afficher le total de bits écrits pour validation
    printf("Total bits ecrits: %lu\n", totalBitsWritten);
}


/**
 * Fonction : decompressFile
 * Description : Décompresse un fichier compressé avec Huffman. Reconstruit l'arbre de Huffman
 *               à partir de la table des fréquences et parcourt les bits pour recréer le contenu original.
 * Paramètres :
 * - const char* inputFile : Nom du fichier compressé en entrée.
 * - const char* outputFile : Nom du fichier décompressé en sortie.
 */

void decompressFile(const char* inputFile, const char* outputFile) {
    printf("Début de la décompression...\n");

    // Ouverture du fichier compressé en mode binaire
    FILE* inFile = fopen(inputFile, "rb");
    if (!inFile) {
        perror("Échec de l'ouverture du fichier d'entrée");
        return;
    }

    // Lecture de la table de fréquences
    int freq[MAX_CHAR];
    if (fread(freq, sizeof(int), MAX_CHAR, inFile) != MAX_CHAR) {
        perror("Échec de la lecture de la table de fréquences");
        fclose(inFile);
        return;
    }

    // Étape 1 : Lire la table de fréquences à partir du fichier compressé et reconstruire l'arbre de Huffman
    char codes[MAX_CHAR][MAX_CHAR] = {0};
    buildHuffmanTree(freq, codes);
    struct MinHeapNode* root = buildTreeFromCodes(codes, freq);

    // Ouverture du fichier de sortie pour écrire les données décompressées
    FILE* outFile = fopen(outputFile, "w");
    if (!outFile) {
        perror("Échec de l'ouverture du fichier de sortie");
        fclose(inFile);
        return;
    }

    // Variables pour le parcours des bits et l'écriture des caractères
    struct MinHeapNode* current = root;
    unsigned long totalBitsRead = 0; // Compteur de bits lus
    unsigned long totalCharsWritten = 0; // Compteur de caractères écrits
    unsigned char byte;

    // Étape 2 : Parcourir les bits du fichier compressé, traverser l'arbre de Huffman, et reconstituer les caractères
    // Lecture des bits et parcours de l'arbre de Huffman
    while (fread(&byte, sizeof(unsigned char), 1, inFile) == 1) {
        for (int i = 7; i >= 0; i--) {  // Parcourt chaque bit du byte
            int bit = (byte >> i) & 1;
            current = bit ? current->right : current->left;
            totalBitsRead++;

            // Si nous atteignons une feuille, écrire le caractère dans le fichier de sortie
            if (!current->left && !current->right) {
                fputc(current->data, outFile);
                totalCharsWritten++;
                current = root;  // Réinitialisation au début de l'arbre
            }
        }
    }

    printf("Décompression terminée.\n");
    printf("Résumé de la décompression :\n");
    printf(" - Total des bits lus : %lu\n", totalBitsRead);
    printf(" - Total des caractères décompressés : %lu\n", totalCharsWritten);

    // Fermeture des fichiers
    fclose(inFile);
    fclose(outFile);
}


/**
 * Fonction : buildTreeFromCodes
 * Description : Reconstruit l'arbre de Huffman à partir des codes de Huffman et des fréquences des caractères.
 * Paramètres :
 * - char codes[MAX_CHAR][MAX_CHAR] : Tableau des codes Huffman pour chaque caractère.
 * - int freq[MAX_CHAR] : Tableau des fréquences des caractères.
 * Retour :
 * - struct MinHeapNode* : Racine de l'arbre de Huffman reconstruit.
 */
struct MinHeapNode* buildTreeFromCodes(char codes[MAX_CHAR][MAX_CHAR], int freq[MAX_CHAR]) {
    struct MinHeapNode* root = newNode('$', 0);  // Créer la racine de l'arbre

    printf("Construction de l'arbre à partir des codes de Huffman...\n");

    // Parcours de tous les caractères pour ajouter des nœuds à l'arbre
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq[i] > 0) { // Si le caractère a une fréquence non nulle
            struct MinHeapNode* current = root;
            char* code = codes[i]; // Code Huffman associé au caractère
            printf("Caractère : '%c' Code : %s\n", (char)i, code);

            // Parcours du code Huffman
            for (int j = 0; code[j] != '\0'; j++) {
                if (code[j] == '0') {
                    if (!current->left) current->left = newNode('$', 0); // Crée un nœud gauche si nécessaire
                    current = current->left; // Descend à gauche
                } else {
                    if (!current->right) current->right = newNode('$', 0); // Crée un nœud droit si nécessaire
                    current = current->right; // Descend à droite
                }
            }
            current->data = (char)i; // Associe le caractère au nœud feuille
            printf("Noeud ajouté pour '%c' avec code %s\n", current->data, code);
        }
    }

    printf("Arbre de Huffman construit avec succès.\n");
    return root; // Retourne la racine de l'arbre
}

// aux pour benchmark
/**
 * getFileSize
 * Description : Récupère la taille d'un fichier en octets.
 * Paramètres :
 * - const char* filename : Le chemin du fichier dont on veut connaître la taille.
 * Retour :
 * - long : La taille du fichier en octets, ou -1 en cas d'erreur.
 */
long getFileSize(const char* filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("Erreur lors de l'obtention de la taille du fichier");
        return -1;
    }
    // Retourner la taille du fichier en octets
    return st.st_size;
}

