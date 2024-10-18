#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>  // Para getrusage
#include <unistd.h>

#define N 1000  // Tamaño de las matrices

double a[N][N], b[N][N], c[N][N];

// Inicializar matrices
void inicializar_matrices() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            a[i][j] = (double) rand() / RAND_MAX;
            b[i][j] = (double) rand() / RAND_MAX;
            c[i][j] = 0;
        }
    }
}

// Multiplicar matrices
void multiplicar_matrices() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Obtener el uso de memoria en kilobytes
long obtener_uso_memoria() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;  // Retorna el uso máximo de memoria en KB
}

// Convertir timeval a nanosegundos
long convertir_a_nanosegundos(struct timeval tv) {
    return (tv.tv_sec * 1000000000L) + (tv.tv_usec * 1000L);
}

int main() {
    srand(time(NULL));

    // Inicializar matrices
    inicializar_matrices();

    // Medir tiempo de ejecución
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);  // Tiempo inicial en nanosegundos

    // Multiplicar matrices
    multiplicar_matrices();

    // Tiempo final
    clock_gettime(CLOCK_MONOTONIC, &end_time);  // Tiempo final en nanosegundos

    // Calcular el tiempo de ejecución en milisegundos
    long execution_time_ms = (end_time.tv_sec - start_time.tv_sec) * 1000L + 
                             (end_time.tv_nsec - start_time.tv_nsec) / 1000000L;

    // Calcular el número de operaciones realizadas (N * N * N multiplicaciones)
    long total_operations = N * N * N;

    // Calcular el tiempo de ejecución por operación en nanosegundos
    long execution_time_per_operation_ns = ((end_time.tv_sec - start_time.tv_sec) * 1000000000L + 
                                           (end_time.tv_nsec - start_time.tv_nsec)) / total_operations;

    // Obtener el uso de memoria en KB
    long memory_usage_kb = obtener_uso_memoria();

    // Obtener el uso de CPU en nanosegundos
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long user_cpu_ns = convertir_a_nanosegundos(usage.ru_utime);  // Tiempo de CPU en modo usuario
    long system_cpu_ns = convertir_a_nanosegundos(usage.ru_stime);  // Tiempo de CPU en modo sistema
    long total_cpu_ns = user_cpu_ns + system_cpu_ns;  // Tiempo total de CPU en nanosegundos

    // Imprimir resultados
    printf("Tiempo total de ejecución: %ld ms\n", execution_time_ms);
    printf("Tiempo de ejecución por operación: %ld ns\n", execution_time_per_operation_ns);
    printf("Uso máximo de memoria: %ld KB\n", memory_usage_kb);
    printf("Tiempo total de CPU: %ld ns\n", total_cpu_ns);

    return 0;
}
