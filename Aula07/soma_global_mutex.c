#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//N constante no código (ii)
#define N 1000000

// A e SUM globais (i)
int A[N];
long long SUM = 0;

// Mutex para proteger a seção crítica de atualização de SUM
pthread_mutex_t mutex;

// Estrutura para passar argumentos para a thread
typedef struct {
    int id;
    int num_threads;
} thread_args;

/**
 * Cada thread calcula a soma de uma fatia do vetor A e a adiciona à
 * variável global SUM, protegendo o acesso com um mutex.
 */
void* soma_parcial(void* arg) {
    thread_args* args = (thread_args*)arg;
    int thread_id = args->id;
    int t = args->num_threads;
    long long soma_local = 0;

    // Divide A em T fatias contíguas
    int fatia_tamanho = N / t;
    int inicio = thread_id * fatia_tamanho;
    int fim;

    // Garante que a última thread processe até o final do vetor
    if (thread_id == t - 1) {
        fim = N;
    } else {
        fim = inicio + fatia_tamanho;
    }

    // Cada thread percorre apenas sua fatia e calcula um parcial local
    for (int i = inicio; i < fim; i++) {
        soma_local += A[i];
    }

    // Seção crítica protegida por um mutex para acumular o parcial em SUM
    // usar exclusivamente pthread_mutex_t (iv)
    pthread_mutex_lock(&mutex);
    SUM += soma_local;
    pthread_mutex_unlock(&mutex);

    free(args);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <T>\n", argv[0]);
        return 1;
    }

    // T pela linha de comando (iii)
    int T = atoi(argv[1]);
    if (T <= 0) {
        printf("O número de threads (T) deve ser um inteiro positivo.\n");
        return 1;
    }

    pthread_t threads[T];

    // Inicializa o vetor A (ex: com todos os elementos iguais a 1)
    for (int i = 0; i < N; i++) {
        A[i] = 1;
    }

    // inicializar corretamente o mutex (viii)
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Falha ao inicializar o mutex");
        return 1;
    }

    // Cria as T threads
    for (int i = 0; i < T; i++) {
        thread_args* args = malloc(sizeof(thread_args));
        if (args == NULL) {
            perror("Falha ao alocar memória para os argumentos da thread");
            return 1;
        }
        args->id = i;
        args->num_threads = T;
        if (pthread_create(&threads[i], NULL, soma_parcial, (void*)args) != 0) {
            perror("Falha ao criar a thread");
            return 1;
        }
    }

    // A main aguarda todas com pthread_join
    for (int i = 0; i < T; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Falha ao aguardar a thread");
            return 1;
        }
    }

    // Calcula a soma sequencial para verificação
    long long SUM_sequencial = 0;
    for (int i = 0; i < N; i++) {
        SUM_sequencial += A[i];
    }

    // imprimir SUM_paralelo, SUM_sequencial e OK/ERRO (vii)
    printf("SUM_paralelo   = %lld\n", SUM);
    printf("SUM_sequencial = %lld\n", SUM_sequencial);

    if (SUM == SUM_sequencial) {
        printf("Resultado: OK\n");
    } else {
        printf("Resultado: ERRO\n");
    }

    // destruir corretamente o mutex (viii)
    pthread_mutex_destroy(&mutex);

    return 0;
}