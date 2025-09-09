#include <pthread.h>
#include <stdio.h>

int p1WantsToEnter = 0;
int p2WantsToEnter = 0;

int counter = 5;
int loops = 0;

void* hello(void* arg) {
    long id = (long)arg;
    while (loops<=10){
        if (id == 0){
            p1WantsToEnter = 1;
            while (p2WantsToEnter==1);// {printf("(T%ld) Esperando... \n", id);}
            // Entra na Secao Critica
            counter++;
            printf("(T%ld) Somei - Counter: %d\n", id, counter);
            loops++;
            p1WantsToEnter = 0;
            // Sai da Secao Critica
        }
        if (id == 1){
            p2WantsToEnter=1;
            while (p1WantsToEnter==1);// {printf("(T%ld) Esperando... \n", id);}
            // Entra na Secao Critica
            counter--;
            printf("(T%ld) Decrementei - Counter: %d\n", id, counter);
            loops++;
            p2WantsToEnter = 0;
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
