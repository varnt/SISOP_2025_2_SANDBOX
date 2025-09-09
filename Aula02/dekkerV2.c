#include <pthread.h>
#include <stdio.h>

int p1Inside = 0;
int p2Inside = 1;

int counter = 5;
int loops = 0;

void* hello(void* arg) {
    long id = (long)arg;
    while (loops<=100){
        if (id == 0){
            while (p2Inside==1);// {printf("(T%ld) Esperando... \n", id);}
            p1Inside = 1;
            // Entra na Secao Critica
            counter++;
            printf("(T%ld) Somei - Counter: %d\n", id, counter);
            p1Inside = 0;
            loops++;
            // Sai da Secao Critica
        }
        if (id == 1){
            while (p1Inside==1);// {printf("(T%ld) Esperando... \n", id);}
            p2Inside = 1;
            // Entra na Secao Critica
            counter--;
            printf("(T%ld) Decrementei - Counter: %d\n", id, counter);
            p2Inside = 0;
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
