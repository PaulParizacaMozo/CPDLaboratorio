#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono; // Para simplificar el uso de chrono

#define MAX 10000

double A[MAX][MAX], x[MAX], y[MAX]; // Declaración de matrices y vectores

void inicializar() {
    for (int i = 0; i < MAX; i++) {
        x[i] = rand() % 100; // Inicializa x con valores aleatorios
        y[i] = 0; // Inicializa y a 0
        for (int j = 0; j < MAX; j++) {
            A[i][j] = rand() % 100; // Inicializa A con valores aleatorios
        }
    }
}

void segundo_bucles() {
    for (int j = 0; j < MAX; j++) {
        for (int i = 0; i < MAX; i++) {
            y[i] += A[i][j] * x[j]; // Segundo bucle
        }
    }
}

int main() {
    inicializar(); // Llama a la función para inicializar las matrices

    auto inicio = high_resolution_clock::now(); // Marca el inicio del tiempo
    segundo_bucles(); // Ejecuta el segundo bucle
    auto fin = high_resolution_clock::now(); // Marca el fin del tiempo

    // Calcula el tiempo transcurrido en milisegundos
    duration<double, std::milli> tiempo_segundo_bucle = fin - inicio;
    cout << "segundo bucle: " << tiempo_segundo_bucle.count() << " ms\n"; // Imprime el tiempo

    return 0; // Fin del programa
}
