#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> 

#define MAX_CMD_LEN 1024

void execute_command(const char *cmd) {
    printf("Exécution : %s\n", cmd);
    int result = system(cmd);
    if (result != 0) {
        printf("Erreur lors de l'exécution de la commande : %s\n", cmd);
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
        printf("Erreur : Impossible d'accéder au dossier '%s'\n", dossier);
        return EXIT_FAILURE;
    }

    
    execute_command("git init");

    
    printf("Voulez-vous ajouter tous les fichiers du répertoire ? (o/n) : ");
    char choix;
    scanf(" %c", &choix);
    getchar(); 

    char cmd[MAX_CMD_LEN];
    if (choix == 'o' || choix == 'O') {
        
        execute_command("git add .");
    } else {
        
        printf("Entrez les noms des fichiers à ajouter (séparés par des espaces) : ");
        char fichiers[MAX_CMD_LEN];
        fgets(fichiers, MAX_CMD_LEN, stdin);
        fichiers[strcspn(fichiers, "\n")] = 0; 

        snprintf(cmd, MAX_CMD_LEN, "git add %s", fichiers);
        execute_command(cmd);
    }

    
    execute_command("git commit -m \"Initial commit\"");

    
    snprintf(cmd, MAX_CMD_LEN, "git remote add origin %s", url_git);
    execute_command(cmd);

    
    execute_command("git branch -M main");
    execute_command("git push -u origin main");

    printf("Dépôt GitHub mis à jour avec succès !\n");
    return EXIT_SUCCESS;
}
