#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

double **A, **B, **C;

// Inicializa las matrices con valores aleatorios
void inicializar(int tamaño) {
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
            C[i][j] = 0;
        }
    }
}

// Multiplica las matrices A y B usando el enfoque en bloques y almacena el resultado en C
void multiplicar_matriz_bloques(int tamaño, int tamaño_bloque) {
    for (int i = 0; i < tamaño; i += tamaño_bloque) {
        for (int j = 0; j < tamaño; j += tamaño_bloque) {
            for (int k = 0; k < tamaño; k += tamaño_bloque) {
                // Multiplicación de bloques
                for (int bi = i; bi < i + tamaño_bloque && bi < tamaño; ++bi) {
                    for (int bj = j; bj < j + tamaño_bloque && bj < tamaño; ++bj) {
                        double suma = 0;
                        for (int bk = k; bk < k + tamaño_bloque && bk < tamaño; ++bk) {
                            suma += A[bi][bk] * B[bk][bj];
                        }
                        C[bi][bj] += suma;
                    }
                }
            }
        }
    }
}

// Función para asignar memoria para las matrices
void asignar_matrices(int tamaño) {
    A = new double*[tamaño];
    B = new double*[tamaño];
    C = new double*[tamaño];
    for (int i = 0; i < tamaño; i++) {
        A[i] = new double[tamaño];
        B[i] = new double[tamaño];
        C[i] = new double[tamaño];
    }
}

// Función para liberar la memoria de las matrices
void liberar_matrices(int tamaño) {
    for (int i = 0; i < tamaño; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <tamaño_matriz> <tamaño_bloque>" << endl;
        return 1;
    }

    int tamaño_matriz = atoi(argv[1]);
    int tamaño_bloque = atoi(argv[2]);
    if (tamaño_matriz <= 0 || tamaño_bloque <= 0) {
        cerr << "El tamaño de la matriz y el bloque deben ser enteros positivos" << endl;
        return 1;
    }

    asignar_matrices(tamaño_matriz);

    inicializar(tamaño_matriz);

    auto inicio = high_resolution_clock::now();
    multiplicar_matriz_bloques(tamaño_matriz, tamaño_bloque);
    auto fin = high_resolution_clock::now();
    duration<double, milli> tiempo_cpu = fin - inicio; // Duración en milisegundos

    cout << "Tiempo de multiplicación de matrices (tamaño bloque " << tamaño_bloque << ") de tamaño " << tamaño_matriz << ": " << tiempo_cpu.count() << " ms" << endl;

    liberar_matrices(tamaño_matriz);

    return 0;
}

