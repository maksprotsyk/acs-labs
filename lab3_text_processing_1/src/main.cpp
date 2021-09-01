#include <iostream>
#include "config.h"
#include "processing.h"

#define DEFAULT_CONFIG "config.dat"


int main(int argc, char **argv) {

    std::string config_file_name = DEFAULT_CONFIG;
    config_t config;
    // we just skip all other arguments
    if (argc == 2) {
        config_file_name = argv[1];
    }
    try {
        config = read_config(config_file_name);
    }
    catch (const std::exception& e) {
        std::cout << "Error while reading config:" << std::endl;
        std::cout << e.what() << std::endl;
        return -2;
    }

    try {
        measurements_t result = index_and_measure_time(config);
        std::cout << result.reading << " " << result.indexing << " " << result.total << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error while starting the processing:" << std::endl;
        std::cout << e.what() << std::endl;
        return -3;
    }

    return 0;
}
