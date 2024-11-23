#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int compteur = 0;  
sem_t semaphore1, semaphore2;  


void ecrire_dans_fichier(int etape, int valeur) {
    FILE* fichier = fopen("resultat.txt", "a");
    if (fichier != NULL) {
        fprintf(fichier, "%d %d\n", etape, valeur); 
        fclose(fichier);
    }
}

void* incrementer(void* arg) {
    int n = *(int*)arg;
    for (int i = -n; i <= n; i++) {
        sem_wait(&semaphore1);  
        compteur += 1;
        ecrire_dans_fichier(i, compteur);  
        printf("Thread 1: %d\n", compteur);
        sem_post(&semaphore1); 
        sem_post(&semaphore2); 
    }
    return NULL;
}

void* decrementer(void* arg) {
    int n = *(int*)arg;
    for (int i = n; i <=2* n; i++) {
        sem_wait(&semaphore2);  
        compteur -= 1;
        ecrire_dans_fichier(n + i, compteur);  
        printf("Thread 2: %d\n", compteur);
        sem_post(&semaphore2);  
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc <2) {
        printf ("Nombre d'argument insuffisant");
        return 1;
    }

    int n = atoi(argv[1]);  

    if (n <= 0) {
        printf("La valeur de n doit être un entier positif.\n");
        return 1;
    }

    pthread_t t1, t2;

    
    sem_init(&semaphore1, 0, 1);  
    sem_init(&semaphore2, 0, 0);  

    
    pthread_create(&t1, NULL, incrementer, &n);
    pthread_create(&t2, NULL, decrementer, &n);

    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);

    printf("Fin du programme.\n");
    return 0;
}
