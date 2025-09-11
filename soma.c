#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000
// Aluno Victor Arnt matricula 00291097
// Variáveis globais
int A[N];
long long SUM = 0;
int T; // Número de threads

void *thread_sum_routine(void *arg)
{
   long id = (long)arg;

   // Calcula a fatia (slice) que esta thread irá processar
   int slice_size = N / T;
   int start = id * slice_size;
   int end = start + slice_size;

   // A última thread pode precisar processar um pouco mais se N não for divisível por T
   if (id == T - 1) {
      end = N;
   }

   // Soma os elementos da sua fatia diretamente na variável global SUM
   // Esta é a seção crítica que causará a condição de corrida
   for (int i = start; i < end; i++) {
      SUM += A[i];
   }

   return NULL;
}

int main(int argc, char **argv) {
   if (argc < 2) {
      printf("Uso: %s <numero_de_threads>\n", argv[0]);
      return 1;
   }

   T = atoi(argv[1]);
   if (T <= 0) {
      printf("O número de threads deve ser um inteiro positivo.\n");
      return 1;
   }

   // Inicializa o vetor A com 1s para facilitar a verificação
   for (int i = 0; i < N; i++) {
      A[i] = 1;
   }

   pthread_t threads[T];
   for (long i = 0; i < T; i++) {
      pthread_create(&threads[i], NULL, thread_sum_routine, (void*)i);
   }

   for (int i = 0; i < T; i++) {
      pthread_join(threads[i], NULL);
   }

   long long sequential_sum = 0;
   for (int i = 0; i < N; i++) {
      sequential_sum += A[i];
   }

   printf("SUM_paralelo   = %lld\n", SUM);
   printf("SUM_sequencial = %lld\n", sequential_sum);
   if (SUM == sequential_sum) {
      printf("Resultado: OK\n");
   } else {
      printf("Resultado: ERRO (Condição de Corrida!)\n");
   }

   return 0;
}

