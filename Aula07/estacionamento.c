#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// Semáforo para controlar o número de vagas disponíveis
sem_t vagas;

// Variável para acompanhar a ocupação e mutex para protegê-la
int ocupacao_atual = 0;
pthread_mutex_t mutex_ocupacao;

// Estrutura para passar argumentos para a thread de cada carro
typedef struct {
    int id;
    int chegada_max_ms;
    int uso_max_ms;
} args_carro;

/**
 * @brief Simula um atraso aleatório em microssegundos.
 * @param max_ms Valor máximo do atraso em milissegundos.
 */
void simular_atraso(int max_ms) {
    if (max_ms > 0) {
        // Converte milissegundos para microssegundos para usleep
        useconds_t atraso = (rand() % (max_ms + 1)) * 1000;
        usleep(atraso);
    }
}

/**
 * @brief Função executada por cada thread (carro).
 *
 * Simula o ciclo de vida de um carro: chega, tenta entrar, usa a vaga e sai.
 */
void* carro(void* arg) {
    args_carro* args = (args_carro*)arg;
    int id = args->id;
    int chegada_max = args->chegada_max_ms;
    int uso_max = args->uso_max_ms;

    // (i) "chega" após um atraso aleatório
    simular_atraso(chegada_max);
    printf("Carro %d chegou ao estacionamento.\n", id);

    // (ii) tenta entrar ocupando uma vaga (primitiva P)
    sem_wait(&vagas); // Bloqueia se não houver vagas

    // Entrou no estacionamento
    pthread_mutex_lock(&mutex_ocupacao);
    ocupacao_atual++;
    printf("Carro %d ENTROU. Ocupação: %d\n", id, ocupacao_atual);
    pthread_mutex_unlock(&mutex_ocupacao);

    // (iii) permanece um tempo aleatório usando a vaga
    simular_atraso(uso_max);

    // (iv) sai, liberando a vaga (primitiva V)
    pthread_mutex_lock(&mutex_ocupacao);
    ocupacao_atual--;
    printf("Carro %d SAIU. Ocupação: %d\n", id, ocupacao_atual);
    pthread_mutex_unlock(&mutex_ocupacao);

    sem_post(&vagas); // Libera a vaga

    free(args);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    // Parâmetros: N (nº de carros), S (vagas) e intervalos (ms) por linha de comando
    if (argc != 5) {
        printf("Uso: %s <N_carros> <S_vagas> <chegada_max_ms> <uso_max_ms>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]); // Número de carros/threads
    int S = atoi(argv[2]); // Número de vagas
    int chegada_max = atoi(argv[3]);
    int uso_max = atoi(argv[4]);

    if (N <= 0 || S <= 0 || chegada_max < 0 || uso_max < 0) {
        printf("Todos os parâmetros devem ser números não negativos, e N e S devem ser maiores que zero.\n");
        return 1;
    }

    pthread_t threads[N];
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    //  usar um semáforo de contagem inicializado com S
    if (sem_init(&vagas, 0, S) != 0) {
        perror("Falha ao inicializar o semáforo");
        return 1;
    }

    if (pthread_mutex_init(&mutex_ocupacao, NULL) != 0) {
        perror("Falha ao inicializar o mutex");
        return 1;
    }

    printf("Estacionamento aberto com %d vagas. %d carros se aproximando...\n", S, N);
    printf("------------------------------------------------------------------\n");

    // Cria as N threads (carros)
    for (int i = 0; i < N; i++) {
        args_carro* args = malloc(sizeof(args_carro));
        args->id = i + 1;
        args->chegada_max_ms = chegada_max;
        args->uso_max_ms = uso_max;
        if (pthread_create(&threads[i], NULL, carro, (void*)args) != 0) {
            perror("Falha ao criar a thread");
            return 1;
        }
    }

    //  finalizar apenas após todas as threads terminarem
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("------------------------------------------------------------------\n");
    printf("Todos os %d carros completaram seu ciclo. Estacionamento fechado.\n", N);

    //  destruir o semáforo ao final
    sem_destroy(&vagas);
    pthread_mutex_destroy(&mutex_ocupacao);

    return 0;
}