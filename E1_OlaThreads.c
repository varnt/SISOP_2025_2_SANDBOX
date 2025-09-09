#include <stdio.h>
#include <pthread.h>

int t;
int i;


void *myThreadFunction(void *arg) {
        //pthread_join(
        int thread_id = *(int *)arg; // Example: retrieve thread ID from argument
        printf("Hello, sou a thread %d\n", thread_id);
        pthread_exit(NULL); // Terminate the thread
}

int main( int argc, char **argv) { 
    double *res;
    int status;
    printf("Digite a quantidade de Threads: ");
    scanf("%d", &t);
    //t++;
    pthread_t threads[t];
    for (i = 0; i < t; i++) {
        threads[i] = i; 
        int rc = pthread_create(&threads[i], NULL, myThreadFunction, (void *)&threads[i]);
        if (rc) {
            fprintf(stderr, "Error creating thread %d: %d\n", i, rc);
            return 1;
        }
    }
	for (i = 0; i < t; i++) {
        threads[i] = i; 
        status = pthread_join(threads[i], (void **) &res);
        if (status !=0) {
            return 1;
        }
        printf("acabando...\n");
    }
   
}
