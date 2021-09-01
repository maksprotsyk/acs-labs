#include <iostream>
#include <string>
#include "integrals.h"
#include "tests.h"
#include "time_measurements.h"

#define BASE_POINTS_NUM 1000
#define MAX_ITERATIONS 4;


int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cout << "Usage ./program configuration_file output_file" << std::endl;
        return -1;
    }

    try {
        program_parameters_t prog_params = read_configuration(argv[1]);
        prog_params.max_iterations = MAX_ITERATIONS;
        auto start = get_current_time_fenced();
        integral_results_t results = complete_integral_calculation(prog_params, test_func, BASE_POINTS_NUM);
        auto finish = get_current_time_fenced();
        results.time = to_us(finish - start);
        write_calculation_results(results, argv[2]);
        std::cout << "Results:" << std::endl;
        std::cout << results.value
        << " " << results.absolute_error
        << " " << results.relative_error
        << " " << results.time << std::endl;
    }
    catch (const std::exception& err) {
        std::cout << "Error" << std::endl;
        std::cout << err.what() << std::endl;
        return -2;
    }

    return 0;

}
