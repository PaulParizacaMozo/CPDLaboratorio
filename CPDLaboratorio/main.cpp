#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define MAX 10000

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

void segundo_bucles() {
    for (int j = 0; j < MAX; j++) {
        for (int i = 0; i < MAX; i++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {
    clock_t inicio, fin;

    inicializar();

    inicio = clock();
    primer_bucles();
    fin = clock();
    cout << "primer bucle: " << (double)(fin - inicio) / CLOCKS_PER_SEC << " segundos\n";

    for (int i = 0; i < MAX; i++) y[i] = 0;

    inicio = clock();
    segundo_bucles();
    fin = clock();
    cout << "segundo bucle: " << (double)(fin - inicio) / CLOCKS_PER_SEC << " segundos\n";

    return 0;
}

