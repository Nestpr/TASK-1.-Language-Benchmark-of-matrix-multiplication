README
How to Run Matrix Multiplication Benchmark

C Language
To compile and run the matrix multiplication code in C, follow these steps:

Navigate to the directory containing your C file:

cd path/to/your/file

Compile the C code with optimization flags:

gcc -O2 -o matrixmultiplication matrixmultiplication.c

Run the compiled program using perf to gather performance statistics:

perf stat ./matrixmultiplication

Python

To run the matrix multiplication benchmark in Python using pytest, execute the following in your terminal:
pytest --benchmark-only -s

Java

For Java, download the JMH plugin in IntelliJ IDEA. Then, to run the matrix multiplication benchmark:

Open the project in IntelliJ.

Click the "Run" button (usually the green triangle icon) to start the benchmark directly.

Make sure JMH dependencies are correctly configured in your project before running the benchmarks.






