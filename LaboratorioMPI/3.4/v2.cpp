#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size, local_value = 1; // Valor inicial en cada proceso
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cálculo de la potencia de dos más cercana
    int power_of_two = 1;
    while (power_of_two <= size / 2) {
        power_of_two *= 2;
    }

    int sum = local_value;

    // Si el proceso está fuera de la potencia de dos, envía su valor a un proceso dentro de la potencia de dos
    if (rank >= power_of_two) {
        MPI_Send(&sum, 1, MPI_INT, rank - power_of_two, 0, MPI_COMM_WORLD);
    } else {
        if (rank + power_of_two < size) {
            int received_value = 0;
            MPI_Recv(&received_value, 1, MPI_INT, rank + power_of_two, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += received_value;
        }

        // Comunicación en mariposa para los procesos dentro de la potencia de dos
        for (int step = 1; step < power_of_two; step *= 2) {
            int partner = rank ^ step;

            int received_value = 0;
            MPI_Sendrecv(&sum, 1, MPI_INT, partner, 0, &received_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            // Sumar el valor recibido al acumulado local
            sum += received_value;
        }
    }

    // El proceso 0 tiene la suma global
    if (rank == 0) {
        global_sum = sum;
        std::cout << "Suma global (comunicación en mariposa para cualquier número de procesos): " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
