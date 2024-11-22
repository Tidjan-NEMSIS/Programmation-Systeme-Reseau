#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> 

#define MAX_CMD_LEN 1024

void execute_command(const char *cmd) {
    printf("Executing: %s\n", cmd);
    int result = system(cmd);
    if (result != 0) {
        printf("Erreur lors de l'execution de la commande : %s\n", cmd);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Utilisation : %s <chemin_du_dossier> <url_du_depot_git>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *dossier = argv[1];
    char *url_git = argv[2];

    
    if (!SetCurrentDirectory(dossier)) {
        printf("Erreur : Impossible d'acceder au dossier '%s'\n", dossier);
        return EXIT_FAILURE;
    }

    // Initialisation du depot Git local
    execute_command("git init");

    // Demander a l'utilisateur s'il veut ajouter tous les fichiers
    printf("Voulez-vous ajouter tous les fichiers du repertoire ? (o/n) : ");
    char choix;
    scanf(" %c", &choix);
    getchar(); // Consomme le saut de ligne restant

    char cmd[MAX_CMD_LEN];
    if (choix == 'o' || choix == 'O') {
        // Ajouter tous les fichiers
        execute_command("git add .");
    } else {
        // Ajouter des fichiers specifiques
        printf("Entrez les noms des fichiers a ajouter (separes par des espaces) : ");
        char fichiers[MAX_CMD_LEN];
        fgets(fichiers, MAX_CMD_LEN, stdin);
        fichiers[strcspn(fichiers, "\n")] = 0; // Retire le saut de ligne

        snprintf(cmd, MAX_CMD_LEN, "git add %s", fichiers);
        execute_command(cmd);
    }

    // Commit initial
    execute_command("git commit -m \"Initial commit\"");

    // Configuration du depot distant
    snprintf(cmd, MAX_CMD_LEN, "git remote add origin %s", url_git);
    execute_command(cmd);

    // Pousser sur la branche principale
    execute_command("git branch -M main");
    execute_command("git push -u origin main");

    printf("Depot GitHub mis a jour avec succes !\n");
    return EXIT_SUCCESS;
}
