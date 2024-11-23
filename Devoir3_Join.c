#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void join(const char *nom_fichier, char **parties, int nb_parties);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage : %s <nom_fichier> <part1> <part2> ... <partN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *nom_fichier = argv[1];
    char **parties = &argv[2];
    int nb_parties = argc - 2;

    join(nom_fichier, parties, nb_parties);
    return 0;
}

void join(const char *nom_fichier, char **parties, int nb_parties) {
    int fd = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Erreur lors de la création du fichier destination");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    for (int i = 0; i < nb_parties; i++) {
        int fd_partie = open(parties[i], O_RDONLY);
        if (fd_partie == -1) {
            perror("Erreur d'ouverture d'une partie");
            close(fd);
            exit(EXIT_FAILURE);
        }

        ssize_t lu;
        while ((lu = read(fd_partie, buffer, sizeof(buffer))) > 0) {
            if (write(fd, buffer, lu) != lu) {
                perror("Erreur d'écriture dans le fichier destination");
                close(fd_partie);
                close(fd);
                exit(EXIT_FAILURE);
            }
        }

        if (lu == -1) {
            perror("Erreur de lecture d'une partie");
            close(fd_partie);
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd_partie);
    }

    close(fd);
}
