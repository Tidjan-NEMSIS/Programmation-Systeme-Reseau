#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void split(const char *nom_fichier, size_t taille);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s <nom_fichier> <taille>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *nom_fichier = argv[1];
    size_t taille = atoi(argv[2]);

    if (taille <= 0) {
        fprintf(stderr, "La taille doit être un entier positif\n");
        exit(EXIT_FAILURE);
    }

    split(nom_fichier, taille);
    return 0;
}

void split(const char *nom_fichier, size_t taille) {
    int fd = open(nom_fichier, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(taille);
    if (!buffer) {
        perror("Erreur d'allocation mémoire");
        close(fd);
        exit(EXIT_FAILURE);
    }

    int compteur = 1;
    ssize_t lu;
    while ((lu = read(fd, buffer, taille)) > 0) {
        char nom_partie[256];
        snprintf(nom_partie, sizeof(nom_partie), "%s.part%d", nom_fichier, compteur);

        int fd_partie = open(nom_partie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_partie == -1) {
            perror("Erreur lors de la création du fichier");
            free(buffer);
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (write(fd_partie, buffer, lu) != lu) {
            perror("Erreur d'écriture dans le fichier de sortie");
            close(fd_partie);
            free(buffer);
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd_partie);
        compteur++;
    }

    if (lu == -1) {
        perror("Erreur de lecture du fichier source");
    }

    free(buffer);
    close(fd);
}
