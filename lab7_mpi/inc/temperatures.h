//
// Created by Maksym Protsyk on 5/20/21.
//

#ifndef MPI_LAB_TEMPERATURES_H
#define MPI_LAB_TEMPERATURES_H

#include "TwoDimArray.h"
#include <cmath>

struct temp_params_t {
    double dx;
    double dy;
    double alpha;
    double dt;
    size_t iterations;
    size_t iterations_per_save;
    std::string input_file;
    std::string output_file;
};


void calculate_temperature(TwoDimArray* old_array_ptr, TwoDimArray* new_array_ptr, const temp_params_t& params);

#endif //MPI_LAB_TEMPERATURES_H
