import numpy as np
import psutil
import os
import time


def matrix_multiplication(A, B):
    process = psutil.Process(os.getpid())

    # Medir el uso de memoria y tiempo de CPU antes de la multiplicación
    memory_before = process.memory_info().rss
    cpu_times_before = process.cpu_times()
    start_time = time.time()  # Registrar el tiempo de inicio

    n = len(A)
    C = np.zeros((n, n))

    # Multiplicación de matrices
    for i in range(n):
        for j in range(n):
            s = 0
            for k in range(n):
                s += A[i][k] * B[k][j]
            C[i][j] = s

    # Medir el uso de memoria y tiempo de CPU después de la multiplicación
    memory_after = process.memory_info().rss
    cpu_times_after = process.cpu_times()
    end_time = time.time()  # Registrar el tiempo de finalización

    # Calcular el tiempo de ejecución
    execution_time = end_time - start_time  # En segundos

    # Calcular el uso de memoria
    memory_usage = memory_after - memory_before  # En bytes

    # Calcular el uso total de CPU (usuario + sistema) en segundos
    cpu_usage_seconds = (cpu_times_after.user + cpu_times_after.system) - (
            cpu_times_before.user + cpu_times_before.system)

    # Convertir el uso de CPU a nanosegundos
    cpu_usage_ns = cpu_usage_seconds * 1e9

    return C, execution_time, memory_usage, cpu_usage_ns


def test_matrix_multiplication():
    # Crear matrices de prueba
    A = np.random.rand(100, 100)
    B = np.random.rand(100, 100)

    # Inicializar variables para acumular las métricas
    total_execution_time = 0
    total_memory_usage = 0
    total_cpu_usage_ns = 0

    warmups = 5
    iterations = 10

    # Iteraciones de calentamiento (no se miden para los promedios)
    for i in range(warmups):
        matrix_multiplication(A, B)
        print(f"Warmup {i + 1} completed.")

    # Iteraciones de medición (se miden para los promedios)
    for i in range(iterations):
        result, execution_time, memory_usage, cpu_usage_ns = matrix_multiplication(A, B)

        total_execution_time += execution_time
        total_memory_usage += memory_usage
        total_cpu_usage_ns += cpu_usage_ns

        # Imprimir resultados para cada iteración
        print(f"Iteration {i + 1}:")
        print(f"Execution time: {execution_time * 1000:.2f} ms")  # Convertimos a milisegundos
        print(f"Memory usage: {memory_usage / 1024:.2f} KB")  # Convertimos a KB
        print(f"CPU usage: {cpu_usage_ns:.0f} ns")  # Uso de CPU en nanosegundos
        print()

    # Calcular los promedios
    avg_execution_time = total_execution_time / iterations
    avg_memory_usage = total_memory_usage / iterations
    avg_cpu_usage_ns = total_cpu_usage_ns / iterations

    # Imprimir promedios
    print(f"\nAverage execution time: {avg_execution_time * 1000:.2f} ms")
    print(f"Average memory usage: {avg_memory_usage / 1024:.2f} KB")
    print(f"Average CPU usage: {avg_cpu_usage_ns:.0f} ns")


# Ejecutar la prueba
test_matrix_multiplication()

