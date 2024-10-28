
#include <mpi.h>
#include <iostream>
#include <ctime>   // Para la función clock()
#include <cmath>   // Para cálculos adicionales si es necesario

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, comm_sz;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Verificar que solo haya dos procesos
    if (comm_sz != 2) {
        if (rank == 0) {
            std::cerr << "Este programa requiere exactamente 2 procesos.\n";
        }
        MPI_Finalize();
        return 1;
    }

    const int num_reps = 10000;  // Número de veces que se repite el ping-pong
    int message = 0;             // Mensaje que se intercambia entre procesos
    MPI_Status status;

    // Medición con la función clock()
    clock_t start_clock, end_clock;
    if (rank == 0) {
        start_clock = clock();
        for (int i = 0; i < num_reps; i++) {
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        }
        end_clock = clock();
        double cpu_time = static_cast<double>(end_clock - start_clock) / CLOCKS_PER_SEC;
        std::cout << "Tiempo usando clock(): " << cpu_time << " segundos\n";
    } else if (rank == 1) {
        for (int i = 0; i < num_reps; i++) {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    // Medición con MPI_Wtime
    double start_mpi, end_mpi;
    if (rank == 0) {
        start_mpi = MPI_Wtime();
        for (int i = 0; i < num_reps; i++) {
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        }
        end_mpi = MPI_Wtime();
        double mpi_time = end_mpi - start_mpi;
        std::cout << "Tiempo usando MPI_Wtime(): " << mpi_time << " segundos\n";
    } else if (rank == 1) {
        for (int i = 0; i < num_reps; i++) {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
