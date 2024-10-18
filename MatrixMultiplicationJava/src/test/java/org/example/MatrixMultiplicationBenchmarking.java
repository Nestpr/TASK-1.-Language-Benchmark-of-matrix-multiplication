package org.example;

import org.openjdk.jmh.annotations.*;

import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;
import java.util.Random;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.SingleShotTime) // Medir en una única ejecución por iteración
@OutputTimeUnit(TimeUnit.MILLISECONDS) // Medir el tiempo en milisegundos
@Fork(1) // Solo un fork
@Warmup(iterations = 5) // 5 iteraciones de calentamiento
@Measurement(iterations = 10) // 10 iteraciones de medición
public class MatrixMultiplicationBenchmarking {

	@State(Scope.Thread)
	public static class Operands {
		private final int n = 10;
		private final double[][] a = new double[n][n];
		private final double[][] b = new double[n][n];

		// Variables para acumular CPU, memoria y tiempo
		public long totalCpuUsage = 0;
		public long totalMemoryUsage = 0;
		public long totalTime = 0;
		public int iterationsCount = 0;

		@Setup
		public void setup() {
			Random random = new Random();
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					a[i][j] = random.nextDouble();
					b[i][j] = random.nextDouble();
				}
			}
		}

		@TearDown(Level.Trial)
		public void printAverages() {
			// Calcular los promedios al finalizar todas las iteraciones de medición
			if (iterationsCount == 10) {
				long avgCpuUsage = totalCpuUsage / iterationsCount;
				long avgMemoryUsage = totalMemoryUsage / iterationsCount;
				long avgTime = totalTime / iterationsCount;

				System.out.println("\nPromedio de CPU usage (ns): " + avgCpuUsage);
				System.out.println("Promedio de Memory usage (KB): " + avgMemoryUsage / 1024);
				System.out.println("Promedio de Execution time (ms): " + avgTime);
			}
		}
	}

	@Benchmark
	public void multiplication(Operands operands) {
		long startTime = System.currentTimeMillis();
		long cpuBefore = getProcessCpuTime();
		long memoryBefore = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();

		executeMatrixMultiplication(operands.a, operands.b);

		long cpuAfter = getProcessCpuTime();
		long memoryAfter = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();
		long endTime = System.currentTimeMillis();

		long cpuUsage = cpuAfter - cpuBefore;
		long memoryUsage = memoryAfter - memoryBefore;
		long timeTaken = endTime - startTime;

		// Acumular los valores en la instancia de Operands
		operands.totalCpuUsage += cpuUsage;
		operands.totalMemoryUsage += memoryUsage;
		operands.totalTime += timeTaken;
		operands.iterationsCount++;

		System.out.println("Iteración " + operands.iterationsCount);
		System.out.println("CPU usage (ns): " + cpuUsage);
		System.out.println("Memory usage (KB): " + memoryUsage / 1024);
		System.out.println("Execution time (ms): " + timeTaken);
	}

	private void executeMatrixMultiplication(double[][] a, double[][] b) {
		int n = a.length;
		double[][] c = new double[n][n];
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}

	private static long getProcessCpuTime() {
		OperatingSystemMXBean osBean = ManagementFactory.getOperatingSystemMXBean();
		if (osBean instanceof com.sun.management.OperatingSystemMXBean) {
			return ((com.sun.management.OperatingSystemMXBean) osBean).getProcessCpuTime();
		} else {
			return -1;
		}
	}
}
