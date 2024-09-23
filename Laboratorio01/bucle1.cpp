#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define MAX 1000

double A[MAX][MAX], x[MAX], y[MAX];

void inicializar() {
    for (int i = 0; i < MAX; i++) {
        x[i] = rand() % 100;
        y[i] = 0;
        for (int j = 0; j < MAX; j++) {
            A[i][j] = rand() % 100;
        }
    }
}

void primer_bucles() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {
    inicializar();

    auto inicio = high_resolution_clock::now();
    primer_bucles();
    auto fin = high_resolution_clock::now();
    duration<double, std::milli> tiempo_primer_bucle = fin - inicio;
    cout << "primer bucle: " << tiempo_primer_bucle.count() << " ms\n";

    return 0;
}

