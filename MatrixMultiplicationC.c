#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>  // Para getrusage
#include <unistd.h>

#define N 1000  // Tamaño de las matrices
#define WARMUP_ITER 5  // Número de iteraciones de calentamiento
#define BENCHMARK_ITER 10  // Número de iteraciones de benchmarking

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

    // Variables para promediar los resultados
    long total_execution_time_ms = 0;
    long total_execution_time_per_op_ns = 0;
    long total_memory_usage_kb = 0;
    long total_cpu_ns = 0;

    // Realizar las iteraciones de calentamiento
    for (int w = 0; w < WARMUP_ITER; ++w) {
        multiplicar_matrices();  // Solo se ejecuta para "calentar"
    }

    // Realizar las iteraciones de benchmarking
    for (int i = 0; i < BENCHMARK_ITER; ++i) {
        // Reiniciar la matriz de resultados
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                c[x][y] = 0;
            }
        }

        // Medir tiempo de ejecución
        struct timespec start_time, end_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);  // Tiempo inicial

        // Multiplicar matrices
        multiplicar_matrices();

        // Tiempo final
        clock_gettime(CLOCK_MONOTONIC, &end_time);  // Tiempo final

        // Calcular el tiempo de ejecución en milisegundos
        long execution_time_ms = (end_time.tv_sec - start_time.tv_sec) * 1000L +
                                 (end_time.tv_nsec - start_time.tv_nsec) / 1000000L;
        total_execution_time_ms += execution_time_ms;

        // Calcular el número de operaciones realizadas (N * N * N multiplicaciones)
        long total_operations = N * N * N;

        // Calcular el tiempo de ejecución por operación en nanosegundos
        long execution_time_per_op_ns = ((end_time.tv_sec - start_time.tv_sec) * 1000000000L +
                                        (end_time.tv_nsec - start_time.tv_nsec)) / total_operations;
        total_execution_time_per_op_ns += execution_time_per_op_ns;

        // Obtener el uso de memoria en KB
        long memory_usage_kb = obtener_uso_memoria();
        total_memory_usage_kb += memory_usage_kb;

        // Obtener el uso de CPU en nanosegundos
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        long user_cpu_ns = convertir_a_nanosegundos(usage.ru_utime);  // Tiempo de CPU en modo usuario
        long system_cpu_ns = convertir_a_nanosegundos(usage.ru_stime);  // Tiempo de CPU en modo sistema
        long total_cpu_ns_iter = user_cpu_ns + system_cpu_ns;  // Tiempo total de CPU en nanosegundos
        total_cpu_ns += total_cpu_ns_iter;
    }

    // Calcular promedios
    long avg_execution_time_ms = total_execution_time_ms / BENCHMARK_ITER;
    long avg_execution_time_per_op_ns = total_execution_time_per_op_ns / BENCHMARK_ITER;
    long avg_memory_usage_kb = total_memory_usage_kb / BENCHMARK_ITER;
    long avg_cpu_ns = total_cpu_ns / BENCHMARK_ITER;

    // Imprimir resultados promedio
    printf("Promedio del tiempo total de ejecución: %ld ms\n", avg_execution_time_ms);
    printf("Promedio del tiempo de ejecución por operación: %ld ns\n", avg_execution_time_per_op_ns);
    printf("Promedio del uso máximo de memoria: %ld KB\n", avg_memory_usage_kb);
    printf("Promedio del tiempo total de CPU: %ld ns\n", avg_cpu_ns);

    return 0;
}
