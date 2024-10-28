
#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size, local_value = 1, global_sum_tree = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificación de potencia de dos
    if ((size & (size - 1)) != 0) {
        if (rank == 0) {
            std::cerr << "Error: el número de procesos debe ser una potencia de dos." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Suma en árbol para potencia de dos
    int step = 1;
    int sum = local_value;

    while (step < size) {
        int partner = rank ^ step;  // Encontrar el proceso compañero usando XOR
        if (partner < size) {
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

    // El proceso 0 tiene la suma global
    if (rank == 0) {
        global_sum_tree = sum;
        std::cout << "Suma global (suma en árbol para potencia de dos): " << global_sum_tree << std::endl;
    }

    MPI_Finalize();
    return 0;
}
