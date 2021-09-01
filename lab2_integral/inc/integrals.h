//
// Created by Maksym Protsyk on 3/3/21.
//

#ifndef INTEGRAL_CALCULATOR_INTEGRALS_H
#define INTEGRAL_CALCULATOR_INTEGRALS_H

#include <vector>
#include <utility>
#include <functional>
#include <vector>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <cmath>
#include <iostream>
#include <fstream>

// parameters that are passed to
// multi-thread integral calculation function
struct integral_parameters_t {
    std::function<double(double, double)> func;
    std::vector<std::pair<double, double>> points1;
    std::vector<std::pair<double, double>> points2;
    std::pair<double, double> step;
};


// arguments that user can pass to our program
// (max_iterations is passed directly and all other
// arguments with the config file)
struct program_parameters_t {
    double x1;
    double x2;
    double y1;
    double y2;
    double absolute_error;
    double relative_error;
    size_t threads;
    size_t max_iterations;
};

// results that we save to a file after calculations
struct integral_results_t {
    double value;
    double absolute_error;
    double relative_error;
    long long time;
};


// function that calculates integral between
// points at given index (in params arguments)
// using one thread only
void calculate_integral(const integral_parameters_t& params,
                        size_t index, double* result, std::mutex& mut);

// performs calculation of the integral using separate thread
// for every pair of points in params argument
double perform_integral_iteration(const integral_parameters_t& params);



// performs iterations until the needed error is reached
integral_results_t complete_integral_calculation(
    const program_parameters_t& prog_params,
    const std::function<double(double, double)>& func,
    size_t steps_num
    );


// reads word from file and checks if it can't be read - throws error
void read_and_check(std::string& str, std::ifstream& file);

// reads configuration file to program_parameters_t
program_parameters_t read_configuration(const std::string& filename);

// appends results to the given file
void write_calculation_results(
    const integral_results_t& res, const std::string& filename
);


#endif //INTEGRAL_CALCULATOR_INTEGRALS_H
