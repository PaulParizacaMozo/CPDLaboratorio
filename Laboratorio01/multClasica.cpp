#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Función para inicializar matrices de tamaño N con valores aleatorios
void inicializar(double **A, double **B, double **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
            C[i][j] = 0;
        }
    }
}

// Función para multiplicar las matrices A y B de tamaño N y almacenar el resultado en C
void multiplicar_matriz(double **A, double **B, double **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Función para asignar memoria para las matrices
double** crear_matriz(int N) {
    double **matriz = new double*[N];
    for (int i = 0; i < N; i++) {
        matriz[i] = new double[N];
    }
    return matriz;
}

// Función para liberar la memoria de las matrices
void liberar_matriz(double **matriz, int N) {
    for (int i = 0; i < N; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <tamaño de matriz>" << endl;
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        cerr << "El tamaño de la matriz debe ser un entero positivo." << endl;
        return 1;
    }

    // Asignar memoria dinámicamente para las matrices
    double **A = crear_matriz(N);
    double **B = crear_matriz(N);
    double **C = crear_matriz(N);

    // Inicializar matrices
    inicializar(A, B, C, N);

    // Medir el tiempo de la multiplicación de matrices
    auto inicio = high_resolution_clock::now();
    multiplicar_matriz(A, B, C, N);
    auto fin = high_resolution_clock::now();
    duration<double, milli> tiempo_cpu = fin - inicio; // Duración en milisegundos

    cout << "Tiempo de multiplicación de matrices de tamaño " << N << ": " << tiempo_cpu.count() << " ms" << endl;

    // Liberar memoria asignada
    liberar_matriz(A, N);
    liberar_matriz(B, N);
    liberar_matriz(C, N);

    return 0;
}
