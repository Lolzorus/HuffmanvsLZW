#include <gtk/gtk.h>
#include <stdio.h>
#include "huffman.h"
#include <time.h> // Pour benchmark du temps

// Callback pour la sélection de fichier à compresser
void on_compress_file(GtkWidget *widget, gpointer user_data) {
    GtkWidget *dialog;
    GtkWindow *parent = GTK_WINDOW(user_data); //utilisation de la librairie GTK (trouver dans le manuel)

    //texte de l'interface graphique
    dialog = gtk_file_chooser_dialog_new("Choisir un fichier à compresser", parent,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Annuler", GTK_RESPONSE_CANCEL,
                                         "_Ouvrir", GTK_RESPONSE_ACCEPT,
                                         NULL);

    //si on clique sur ouvrir
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        clock_t start = clock();

        // On appelle la fonction de compression ici
        compressFile(filename, "compressed.bin");
        clock_t end = clock();

        double timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Temps d'exécution : %.2f secondes\n", timeTaken);

        g_print("Fichier compressé avec succés.\n");

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

// Callback pour la sélection de fichier à décompresser (meme logique que pour compresser)
void on_decompress_file(GtkWidget *widget, gpointer user_data) {
    GtkWidget *dialog;
    GtkWindow *parent = GTK_WINDOW(user_data);

    dialog = gtk_file_chooser_dialog_new("Choisir un fichier à decompresser", parent,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Annuler", GTK_RESPONSE_CANCEL,
                                         "_Ouvrir", GTK_RESPONSE_ACCEPT,
                                         NULL);

    //si on clique sur ouvrir
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        clock_t start = clock();

        // Appel de la fonction de décompression ici
        decompressFile(filename, "output.txt");
        clock_t end = clock();

        double timeTaken = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Temps d'exécution : %.2f secondes\n", timeTaken);

        g_print("Fichier décompressé avec succés.\n");

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *compress_button;
    GtkWidget *decompress_button;

    gtk_init(&argc, &argv);

    // Créer la fenêtre principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Compression de Huffman");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Créer une grille pour les boutons
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Bouton pour compresser un fichier
    compress_button = gtk_button_new_with_label("Compresser le fichier");
    g_signal_connect(compress_button, "clicked", G_CALLBACK(on_compress_file), window);
    gtk_grid_attach(GTK_GRID(grid), compress_button, 0, 0, 1, 1);

    // Bouton pour décompresser un fichier
    decompress_button = gtk_button_new_with_label("Decompresser le fichier");
    g_signal_connect(decompress_button, "clicked", G_CALLBACK(on_decompress_file), window);
    gtk_grid_attach(GTK_GRID(grid), decompress_button, 1, 0, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
