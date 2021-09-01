//
// Created by Maksym Protsyk on 5/21/21.
//

#ifndef MPI_LAB_MPI_MODULES_H
#define MPI_LAB_MPI_MODULES_H

#include <boost/mpi.hpp>
#include <iostream>

#include "temperatures.h"
#include <boost/program_options.hpp>

const int height_data = 2;
const int width_data = 3;
const int rows_data = 4;
const int upper_row = 5;
const int lower_row = 6;
const int save_data = 7;


void io_thread(const boost::mpi::communicator& world, const temp_params_t& params);


void processing_thread(const boost::mpi::communicator& world, const temp_params_t& params);

#endif //MPI_LAB_MPI_MODULES_H
