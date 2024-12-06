# Implémentation des algorithmes de compression : Huffman et LZW

# Implémentation des algorithmes
# Technologies utilisées
Langage : C pour sa gestion fine des ressources.
Outils : GCC pour la compilation.
Formats : Fichiers texte (.txt) pour simplifier les tests.

# Implémentation d'une interface graphique pour l'algorithme de Huffman avec GTK+3.

# Description sommaire des implémentations
# Algorithme de Huffman

# Compression :
Analyse des fréquences des caractères.
Construction de l’arbre binaire.
Génération des codes binaires.

# Décompression :
Reconstruction de l’arbre depuis les métadonnées.
Parcours de l’arbre pour décoder les données.

# Algorithme LZW
# Compression :
Initialisation d’un dictionnaire avec la liste des caractères uniques.
Ajout dynamique des motifs au fur et à mesure de la lecture.

# Décompression :
Utilisation des indices pour reconstruire les séquences.
Mise à jour du dictionnaire à chaque étape.

# Structure du répertoire des codes sources et commande GCC pour exécution
Il y a 3 répertoires :
Un pour l’algorithme de Huffman.
Un pour LZW.
Un pour les captures d’écrans.

# Les deux répertoires d’algorithmes contiennent chacun :
2 fichiers .txt : Un fichier de 3,5 Mo (10M.txt) et un fichier AAA.txt pour tester des données répétitives.
Un fichier « instruction.txt » contenant les lignes GCC pour les programmes.
Les codes sources.

# Instructions et utilisation de l'algorithme de Huffman
L'algorithme de Huffman utilise la bibliothèque GTK+ 3 pour générer une interface graphique.

# Pour Ubuntu, installez GTK+ 3 via :
sudo apt install libgtk-3-dev

# Pour compiler (instructions situées dans instruction.txt) :
gcc `pkg-config --cflags gtk+-3.0` -o huffman_gui huffman.c main.c `pkg-config --libs gtk+-3.0`

# Exécution :
./huffman_gui

# Utilisation :
L'interface permet de compresser un fichier en cliquant sur un bouton et de choisir le fichier .txt à compresser. Pour décompresser, il suffit de cliquer sur "Décompresser" et de sélectionner le fichier .bin.

# Instructions pour LZW (sans interface graphique) :
# Pour compiler (instructions situées dans instruction.txt) :
gcc lzw.c par.c -o project

# Exécution :
./project

# Utilisation :
Le programme fonctionne en ligne de commande. Choisissez "C" pour compresser ou "D" pour décompresser, puis entrez le nom du fichier à compresser et le fichier de sortie.

# Analyse comparative simple
Taux de compression : Huffman est plus performant sur les données aléatoires (2,000,000 octets contre 2,750,000 pour LZW).
Temps d’exécution : Huffman est 5 fois plus rapide lors de la compression (0,25 seconde contre 1,45 seconde) mais légèrement plus lent pour la décompression.

# Interprétation
Les tests ont montré des différences notables :
Taux de compression : Huffman est plus efficace pour les données aléatoires, tandis que LZW est meilleur pour les données répétitives.
Temps d’exécution : Huffman est plus rapide à la compression, mais LZW est ralenti par la gestion de son dictionnaire.
Utilisation mémoire : LZW consomme plus de mémoire et est plus lent sur de gros fichiers par rapport à Huffman.

# Conclusion :
L’algorithme de Huffman est généralement plus performant, surtout pour les données avec des fréquences variées.
LZW est plus simple et utilise un dictionnaire, mais consomme plus de mémoire et est plus adapté aux séquences répétitives.

# Améliorations potentielles :
Optimisation des structures de données pour les algorithmes.
Exploration d’autres algorithmes comme BWT.
Intégration de métriques de performance pour des configurations matérielles différentes.
Transformation du programme en librairie pour une utilisation facile.

## English
# Compression Algorithms Implementation: Huffman and LZW

# Algorithm Implementation
# Technologies Used
Language: C for its fine-grained resource management.
Tools: GCC for compilation.
Formats: Text files (.txt) for simplifying tests.
Bonus: GUI implementation for the Huffman algorithm with GTK+3.

# Summary of Implementations
# Huffman Algorithm

# Compression:
Analyze character frequencies.
Build the binary tree.
Generate binary codes.

# Decompression:
Rebuild the tree from metadata.
Traverse the tree to decode the data.

# LZW Algorithm
# Compression:
Initialize a dictionary with the list of unique characters.
Dynamically add patterns as data is read.

# Decompression:
Use indices to reconstruct sequences.
Update the dictionary at each step.

# Directory Structure and GCC Commands for Execution
# There are 3 directories:
One for the Huffman algorithm.
One for LZW.
One for screenshots.

# Each algorithm directory contains:
2 .txt files: A 3.5 MB file (10M.txt) and an AAA.txt file for testing repetitive data.
An "instruction.txt" file with the GCC commands for compiling the programs.
2 The source code.

# Instructions and Usage for the Huffman Algorithm
The Huffman algorithm uses GTK+ 3 to generate a graphical interface.

# On Ubuntu, install GTK+ 3 with:
sudo apt install libgtk-3-dev

# To compile (instructions in instruction.txt):
gcc `pkg-config --cflags gtk+-3.0` -o huffman_gui huffman.c main.c `pkg-config --libs gtk+-3.0`

# Run:
./huffman_gui

# Usage:
The interface allows you to compress a file by clicking a button to select the .txt file to compress. To decompress, click "Decompress" and select the .bin file.

# LZW Instructions (No graphical interface):
# To compile (instructions in instruction.txt):
gcc lzw.c par.c -o project

# Run:
./project

# Usage:
The program works via command line. Choose "C" to compress or "D" to decompress, then enter the name of the file to compress and the output file.

# Simple Comparative Analysis
Compression Rate: Huffman is more efficient on random data (2,000,000 bytes vs. 2,750,000 for LZW).
Execution Time: Huffman is 5 times faster at compression (0.25 seconds vs. 1.45 seconds) but slightly slower during decompression.

# Interpretation
The tests showed notable differences:
Compression Rate: Huffman is more efficient for random data, while LZW performs better on repetitive data.
Execution Time: Huffman is faster at compression, but LZW is slowed down by its dictionary management.
Memory Usage: LZW consumes more memory and is slower on large files compared to Huffman.

# Conclusion:
The Huffman algorithm is generally more efficient, especially for data with varying frequencies.
LZW is simpler and uses a dictionary but consumes more memory and is better suited for repetitive sequences.

# Potential Improvements:
Optimizing data structures for the algorithms.
Exploring other algorithms like BWT.
Integrating performance metrics for different hardware setups.
Converting the program into a library for easier use.





