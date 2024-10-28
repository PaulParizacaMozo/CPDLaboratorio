
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int find_bin(float value, const std::vector<float>& bin_maxes, float min_meas) {
    int bin = 0;
    while (bin < bin_maxes.size() - 1 && value >= bin_maxes[bin]) {
        bin++;
    }
    return bin;
}

void initialize_bin_maxes(std::vector<float>& bin_maxes, int bin_count, float min_meas, float max_meas) {
    float bin_width = (max_meas - min_meas) / bin_count;
    for (int b = 0; b < bin_count; b++) {
        bin_maxes[b] = min_meas + bin_width * (b + 1);
    }
}

// Función para que el proceso 0 lea los datos del archivo y los cargue en el vector
void load_data_from_file(const std::string& filename, std::vector<float>& data, int& data_count, int rank) {
    if (rank == 0) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        float value;
        while (infile >> value) {
            data.push_back(value);
        }
        infile.close();

        data_count = data.size();
        if (data_count != 100000) {
            std::cerr << "Error: Se esperaban 100,000 datos, pero se encontraron " << data_count << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    std::vector<float> data;
    int data_count = 0;

    float min_meas = 0.0, max_meas = 5.0;
    int bin_count = 5;
    std::vector<int> global_bin_counts(bin_count, 0);
    std::vector<int> local_bin_counts(bin_count, 0);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Proceso 0 carga los datos desde el archivo
    std::string filename = "data.txt";  // Nombre del archivo de entrada
    load_data_from_file(filename, data, data_count, rank);

    // El proceso 0 distribuye el tamaño de los datos
    MPI_Bcast(&data_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Redimensionamos el vector local si no es el proceso 0
    if (rank != 0) {
        data.resize(data_count);
    }

    // División de datos entre procesos
    int data_per_process = data_count / size;
    std::vector<float> local_data(data_per_process);
    MPI_Scatter(data.data(), data_per_process, MPI_FLOAT, local_data.data(), data_per_process, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Inicializamos los bin_maxes para el histograma
    std::vector<float> bin_maxes(bin_count);
    initialize_bin_maxes(bin_maxes, bin_count, min_meas, max_meas);

    // Sincronizamos todos los procesos antes de empezar la medición
    MPI_Barrier(MPI_COMM_WORLD);

    // Medir el tiempo de cálculo
    double start_time = MPI_Wtime();

    // Cada proceso calcula su histograma local
    for (int i = 0; i < data_per_process; i++) {
        int bin = find_bin(local_data[i], bin_maxes, min_meas);
        local_bin_counts[bin]++;
    }

    // Medir el tiempo después del cálculo local
    double end_time = MPI_Wtime();

    // Convertir a milisegundos
    double elapsed_time_ms = (end_time - start_time) * 1000.0;

    // Reducción para obtener el histograma global
    MPI_Reduce(local_bin_counts.data(), global_bin_counts.data(), bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Proceso 0 imprime el histograma global y el tiempo de cálculo en milisegundos
    if (rank == 0) {
        std::cout << "Histograma global:\n";
        for (int i = 0; i < bin_count; i++) {
            std::cout << "Bin " << i << ": " << global_bin_counts[i] << std::endl;
        }
        std::cout << "Tiempo de cálculo del histograma: " << elapsed_time_ms << " milisegundos\n";
    }

    MPI_Finalize();
    return 0;
}
