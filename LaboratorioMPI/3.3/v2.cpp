
#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size, local_value = 1, global_sum_tree = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Fase preliminar para ajustar el tamaño a la potencia de dos más cercana
    int next_power_of_two = 1;
    while (next_power_of_two < size) {
        next_power_of_two *= 2;
    }

    int sum = local_value;

    // Procesos fuera de la potencia de dos envían su valor a un proceso en el rango de potencia de dos
    if (rank >= next_power_of_two / 2) {
        int partner = rank - next_power_of_two / 2;
        MPI_Send(&sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
    } else {
        if (rank + next_power_of_two / 2 < size) {
            int received_value = 0;
            MPI_Recv(&received_value, 1, MPI_INT, rank + next_power_of_two / 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += received_value;
        }

        // Estructura en árbol para la potencia de dos
        int step = 1;
        while (step < next_power_of_two / 2) {
            int partner = rank ^ step;
            if (partner < next_power_of_two / 2) {
                int received_value = 0;
                if (rank < partner) {
                    MPI_Recv(&received_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    sum += received_value;
                } else {
                    MPI_Send(&sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
                }
            }
            step *= 2;
        }
    }

    // El proceso 0 tiene la suma global
    if (rank == 0) {
        global_sum_tree = sum;
        std::cout << "Suma global (suma en árbol para cualquier número de procesos): " << global_sum_tree << std::endl;
    }

    MPI_Finalize();
    return 0;
}
