#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size, local_value = 1; // Valor inicial en cada proceso
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificación de que el número de procesos es potencia de dos
    if ((size & (size - 1)) != 0) {
        if (rank == 0) {
            std::cerr << "Error: el número de procesos debe ser una potencia de dos." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int sum = local_value;
    for (int step = 1; step < size; step *= 2) {
        int partner = rank ^ step;  // Calcula el proceso compañero con XOR

        int received_value = 0;
        MPI_Sendrecv(&sum, 1, MPI_INT, partner, 0, &received_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Sumar el valor recibido al acumulado local
        sum += received_value;
    }

    // El proceso 0 almacena la suma global después de la última etapa
    if (rank == 0) {
        global_sum = sum;
        std::cout << "Suma global (comunicación en mariposa para potencia de dos): " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
