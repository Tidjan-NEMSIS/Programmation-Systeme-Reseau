#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h>


int **creer_matrice(int dimension);
void afficher_matrice(int **matrice, int dimension);
void enregistrer_matrice_texte(int **matrice, int dimension, const char *nom_fichier);
void enregistrer_matrice_binaire(int **matrice, int dimension, const char *nom_fichier);
int **charger_matrice_texte(int dimension, const char *nom_fichier);
int **charger_matrice_binaire(int dimension, const char *nom_fichier);
void liberer_matrice(int **matrice, int dimension);

int main(int argc, char *argv[]) {
    int creer = 0, afficher = 0, binaire = 1;
    int dimension = 0;
    char *nom_fichier = NULL;
    int opt;


    while ((opt = getopt(argc, argv, "cad:f:bt")) != -1) {
        switch (opt) {
            case 'c':
                creer = 1;
                break;
            case 'a':
                afficher = 1;
                break;
            case 'd':
                dimension = atoi(optarg);
                break;
            case 'f':
                nom_fichier = optarg;
                break;
            case 'b':
                binaire = 1;
                break;
            case 't':
                binaire = 0;
                break;
            default:
                fprintf(stderr, "Option invalide\n");
                exit(EXIT_FAILURE);
        }
    }


    if (dimension <= 0) {
        fprintf(stderr, "La dimension doit être spécifiée avec -d et être un entier positif\n");
        exit(EXIT_FAILURE);
    }

    if (nom_fichier == NULL) {
        fprintf(stderr, "Le nom du fichier doit être spécifié avec -f\n");
        exit(EXIT_FAILURE);
    }

    int **matrice = NULL;

    if (creer) {
        matrice = creer_matrice(dimension);
        if (binaire) {
            enregistrer_matrice_binaire(matrice, dimension, nom_fichier);
        } else {
            enregistrer_matrice_texte(matrice, dimension, nom_fichier);
        }
    }

    if (afficher) {
        if (binaire) {
            matrice = charger_matrice_binaire(dimension, nom_fichier);
        } else {
            matrice = charger_matrice_texte(dimension, nom_fichier);
        }
        afficher_matrice(matrice, dimension);
    }

    if (matrice) {
        liberer_matrice(matrice, dimension);
    }

    return 0;
}


int **creer_matrice(int dimension) {
    int **matrice = malloc(dimension * sizeof(int *));
    if (!matrice) {
        perror("Erreur d'allocation mémoire pour les pointeurs");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dimension; i++) {
        matrice[i] = malloc(dimension * sizeof(int));
        if (!matrice[i]) {
            perror("Erreur d'allocation mémoire pour une ligne");
            exit(EXIT_FAILURE);
        }
    }

    srand(time(NULL));
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            matrice[i][j] = rand() % 100;
        }
    }

    return matrice;
}


void afficher_matrice(int **matrice, int dimension) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}


void enregistrer_matrice_texte(int **matrice, int dimension, const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier texte");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            fprintf(fichier, "%d ", matrice[i][j]);
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
}


void enregistrer_matrice_binaire(int **matrice, int dimension, const char *nom_fichier) {
    int fd = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (fd == -1) {
        perror("Erreur lors de l'ouverture du fichier binaire");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dimension; i++) {
        if (write(fd, matrice[i], dimension * sizeof(int)) != dimension * sizeof(int)) {
            perror("Erreur lors de l'écriture dans le fichier binaire");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    close(fd);
}


int **charger_matrice_texte(int dimension, const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier texte");
        exit(EXIT_FAILURE);
    }

    int **matrice = creer_matrice(dimension);

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (fscanf(fichier, "%d", &matrice[i][j]) != 1) {
                perror("Erreur de lecture du fichier texte");
                fclose(fichier);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(fichier);
    return matrice;
}


int **charger_matrice_binaire(int dimension, const char *nom_fichier) {
    int fd = open(nom_fichier, O_RDONLY);
    if (fd == -1) {
        perror("Erreur lors de l'ouverture du fichier binaire");
        exit(EXIT_FAILURE);
    }

    int **matrice = creer_matrice(dimension);

    for (int i = 0; i < dimension; i++) {
        if (read(fd, matrice[i], dimension * sizeof(int)) != dimension * sizeof(int)) {
            perror("Erreur de lecture du fichier binaire");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    close(fd);
    return matrice;
}


void liberer_matrice(int **matrice, int dimension) {
    for (int i = 0; i < dimension; i++) {
        free(matrice[i]);
    }
    free(matrice);
}
