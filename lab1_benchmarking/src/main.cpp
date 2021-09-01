#include <iostream>
#include <vector>
#include <string>
#include "fileio.h"
#include "methods.h"
#include "measurements.h"


enum errors{BAD_ARGS_AMOUNT=-1, INVALID_ARGS=-2, FILE_ERROR=-3};

std::vector<method_t> methods{
    stod_method,
    lexical_cast_method,
    stream_method
};
std::vector<std::string> method_names{
    "STOD",
    "LEXICAL CAST",
    "STRINGSTREAM"
};

// default path which is added before entered filepath
std::string DEFAULT_PATH = "";


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage ./program method_num input_file output_file" << std::endl;
        return BAD_ARGS_AMOUNT;
    }

    size_t method_num;
    try {
        method_num = std::stoi(argv[1]);
    }
    catch (std::invalid_argument& e) {
        std::cout << "Invalid method arg" << std::endl;
        return INVALID_ARGS;
    }
    catch (std::out_of_range& e) {
        std::cout << "Invalid method arg" << std::endl;
        return INVALID_ARGS;
    }

    if (method_num < 1 || method_num > methods.size()) {
        std::cout << "Invalid method num" << std::endl;
        return INVALID_ARGS;
    }
    std::vector<std::string> words;

    try {
        words = read_by_word(DEFAULT_PATH + std::string(argv[2]));
    }
    catch (BadFileException& e) {
        std::cout << "Can't open input file" << std::endl;
        return FILE_ERROR;
    }

    // measuring time
    auto start = get_current_time_fenced();
    std::vector<double> numbers = methods[method_num - 1](words);
    auto finish = get_current_time_fenced();
    long long time = to_us(finish - start);
    try {
        write_results(
            DEFAULT_PATH + std::string(argv[3]),
            numbers,
            time
            );
    }
    catch (BadFileException& e) {
        std::cout << "Can't open output file" << std::endl;
        return FILE_ERROR;
    }

    std::cout << "Time of " << method_names[method_num - 1] << " method:" << std::endl;
    std::cout << time << std::endl;

    return 0;
}


