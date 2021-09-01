//
// Created by Maksym Protsyk on 2/18/21.
//
#include "fileio.h"

std::vector<std::string> read_by_word(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw BadFileException();
    }
    std::string current;
    std::vector<std::string> words;
    while(file >> current){
        words.push_back(current);
    }
    file.close();
    return words;
}

void write_results(const std::string& filename,
                   const std::vector<double>& numbers,
                   long long time) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        throw BadFileException();
    }
    double num_sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
    file << num_sum << " " << num_sum/numbers.size() << " " << time << std::endl;
    file.close();
}