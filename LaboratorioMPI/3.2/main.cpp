
#include <mpi.h>
#include <iostream>
#include <cstdlib> // Para rand y RAND_MAX
#include <ctime>   // Para time
#include <cmath>   // Para sqrt

int main(int argc, char** argv) {
    int rank, size;
    long long int number_of_tosses = 0;
    long long int number_in_circle = 0;
    long long int global_number_in_circle = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Leer el número total de lanzamientos
        std::cout << "Ingrese el número total de lanzamientos: " << std::endl;
        std::cin >> number_of_tosses;
    }

    // Distribuir el número total de lanzamientos a todos los procesos
    MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    // Calcular el número de lanzamientos por proceso
    long long int local_tosses = number_of_tosses / size;

    // Inicializar la semilla para el generador de números aleatorios
    std::srand(static_cast<unsigned int>(std::time(0)) + rank);

    // Medir el tiempo de cálculo
    double start_time = MPI_Wtime();

    // Realizar los lanzamientos locales
    for (long long int toss = 0; toss < local_tosses; toss++) {
        double x = (static_cast<double>(std::rand()) / RAND_MAX) * 2.0 - 1.0;
        double y = (static_cast<double>(std::rand()) / RAND_MAX) * 2.0 - 1.0;
        double distance_squared = x * x + y * y;
        if (distance_squared <= 1.0) {
            number_in_circle++;
        }
    }

    // Sumar todos los éxitos en el círculo
    MPI_Reduce(&number_in_circle, &global_number_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Medir el tiempo de finalización
    double end_time = MPI_Wtime();

    // El proceso 0 estima y muestra π junto con el tiempo de ejecución
    if (rank == 0) {
        double pi_estimate = 4.0 * static_cast<double>(global_number_in_circle) / static_cast<double>(number_of_tosses);
        double elapsed_time = (end_time - start_time) * 1000.0; // Convertir a milisegundos
        std::cout << "Estimación de π = " << pi_estimate << std::endl;
        std::cout << "Tiempo de cálculo: " << elapsed_time << " milisegundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
