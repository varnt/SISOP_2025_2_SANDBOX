#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

volatile int p1WantsToEnter = 0;
volatile int p2WantsToEnter = 0;

volatile int turn = 1;   // 1 = vez da T1 (id=1), 2 = vez da T0 (id=0)

volatile int counter = 5;
volatile int loops   = 0;

void* hello(void* arg) {
    long id = (long)arg;
    while (loops<=10){
        if (id == 0){
            p1WantsToEnter = 1;
            while (p2WantsToEnter==1){
                if (turn == 1){
                    p1WantsToEnter = 0;
                    while (turn==1);
                    p1WantsToEnter = 1;
                }
            }
            // Seção Crítica
            counter++;
            printf("(T%ld) Somei - Counter: %d\n", id, counter);
            turn = 1;                      // <- entrega a vez ao outro (T1)
            p1WantsToEnter = 0;
            loops++;
        }
        if (id == 1){
            p2WantsToEnter=1;
            while (p1WantsToEnter==1){
                if (turn == 0){
                    p2WantsToEnter = 0;
                    while (turn==0);
                    p2WantsToEnter = 1;
                }
            }
            // Seção Crítica
            counter--;
            printf("(T%ld) Decrementei - Counter: %d\n", id, counter);
            turn = 0;
            p2WantsToEnter = 0;
            loops++;
        }
    }
    return NULL;
}

int main(void) {
    pthread_t t[2];
    srand(time(NULL));
    for (long i = 0; i < 2; i++)
        pthread_create(&t[i], NULL, hello, (void*)i);

    for (int i = 0; i < 2; i++)
        pthread_join(t[i], NULL);

    return 0;
}
