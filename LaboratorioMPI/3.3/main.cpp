
#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    int rank, size, local_value = 1; // Todos los procesos tienen un valor local de 1
    int global_sum_tree = 0, global_sum_reduce = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Suma en árbol
    double start_tree = MPI_Wtime();
    int step = 1;
    int sum = local_value;

    while (step < size) {
        int partner = rank ^ step;  // XOR para encontrar el proceso compañero
        if (partner < size) {
            int received_value = 0;
            if (rank < partner) {
                // Recibir el valor del compañero y sumarlo al valor local
                MPI_Recv(&received_value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sum += received_value;
            } else {
                // Enviar el valor local al compañero
                MPI_Send(&sum, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            }
        }
        step *= 2;
    }

    if (rank == 0) global_sum_tree = sum;
    double end_tree = MPI_Wtime();

    // Suma con MPI_Reduce
    double start_reduce = MPI_Wtime();
    MPI_Reduce(&local_value, &global_sum_reduce, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double end_reduce = MPI_Wtime();

    // Resultados y tiempos
    if (rank == 0) {
        std::cout << "Suma global (suma en árbol): " << global_sum_tree
                  << ", tiempo: " << (end_tree - start_tree) * 1000 << " ms" << std::endl;
        std::cout << "Suma global (MPI_Reduce): " << global_sum_reduce
                  << ", tiempo: " << (end_reduce - start_reduce) * 1000 << " ms" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
