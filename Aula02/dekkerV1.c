// two_threads_hello.c
#include <pthread.h>
#include <stdio.h>

int turn = 0;
int counter = 5;
int loops = 0;

void* hello(void* arg) {
    long id = (long)arg;
    while (loops<=10){
        if (id == 0){
            while (turn == 1) {printf("(T%ld) Esperando... \n", id);}
            // Entra na Secao Critica
            counter++;
            printf("(T%ld) Somei - Counter: %d\n", id, counter);
            turn = 1;
            loops++;
            // Sai da Secao Critica
        }
        if (id == 1){
            while (turn == 0) {printf("(T%ld) Esperando... \n", id);}
            // Entra na Secao Critica
            counter--;
            printf("(T%ld) Decrementei - Counter: %d\n", id, counter);
            turn = 0;
            loops++;
            // Sai da Secao Critica

        }
    }

    return NULL;
}

int main(void) {
    pthread_t t[2];

    for (long i = 0; i < 2; i++)
        pthread_create(&t[i], NULL, hello, (void*)i);

    for (int i = 0; i < 2; i++)
        pthread_join(t[i], NULL);

    return 0;
}
