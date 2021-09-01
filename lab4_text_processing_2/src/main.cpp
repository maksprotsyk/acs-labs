#include <iostream>
#include "config.h"
#include "processing.h"

#define DEFAULT_CONFIG "config.dat"


int main(int argc, char **argv) {

    // default parameters
    std::string config_file_name = DEFAULT_CONFIG;
    config_t config;
    config.index_directory_path = ".";
    config.by_alphabet_path = "alphabet.txt";
    config.by_count_path = "count.txt";
    config.threads_indexing = 12;
    config.threads_merging = 12;
    config.text_file_formats = {".txt"};
    config.archive_file_formats = {".zip"};
    config.max_size = 10000000;
    config.index_queue_size = 250;
    config.merge_queue_size = 500;

    // we just skip all other arguments
    if (argc == 2) {
        config_file_name = argv[1];
    }
    try {
        read_config(config, config_file_name);
    }
    catch (const std::exception& e) {
        std::cout << "Error while reading config:" << std::endl;
        std::cout << e.what() << std::endl;
        return -2;
    }

    try {
        measurements_t result = index_and_measure_time(config);
        std::cout << result.io << " " << result.indexing << " " << result.total << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error while starting the processing:" << std::endl;
        std::cout << e.what() << std::endl;
        return -3;
    }


    return 0;

}
