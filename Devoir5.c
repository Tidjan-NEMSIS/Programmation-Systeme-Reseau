#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define Max 10

sem_t semaphores[Max];
int n;

void* Me_Voila(void* arg) {
    int thread_id = *(int*)arg;
    while (1) {
        sem_wait(&semaphores[thread_id]);
        printf("Thread %d: Je m'exécute !\n", thread_id + 1);
        sem_post(&semaphores[(thread_id + 1) % n]);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Comme ça: %s <nombre de threads>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n <= 0 || n > Max) {
        printf("Le nombre de threads doit être compris entre 1 et %d.\n", Max);
        return 1;
    }

    pthread_t threads[n];
    int thread_ids[n];

    for (int i = 0; i < n; i++) {
        sem_init(&semaphores[i], 0, 0);
        thread_ids[i] = i;
    }

    sem_post(&semaphores[0]);

    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, Me_Voila, &thread_ids[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < n; i++) {
        sem_destroy(&semaphores[i]);
    }

    return 0;
}
