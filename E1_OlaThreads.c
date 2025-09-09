#include <stdio.h>
#include <pthread.h>

int t;
int i;
void *thread_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;


void *thread_function(void *dummyPtr)
{
   printf("Hello, sou a thread: %ld\n", pthread_self());
}

int main( int argc, char **argv) { 
    double *res;
    int status;
    printf("Digite a quantidade de Threads: ");
    scanf("%d", &t);
     pthread_t thread_id[t];
   int i, j;

   for(i=0; i < t; i++)
   {
      pthread_create( &thread_id[i], NULL, thread_function, NULL );
   }

   for(j=0; j < t; j++)
   {
      pthread_join( thread_id[j], NULL); 
   }
  
   /* Now that all threads are complete I can print the final result.     */
   /* Without the join I could be printing a value before all the threads */
   /* have been completed.                                                */

   printf("Final counter value: %d\n", j);
   
}
