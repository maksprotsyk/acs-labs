//
// Created by Maksym Protsyk on 2/17/21.
//

#ifndef PRODUCTIVITY_TEST_FILEIO_H
#define PRODUCTIVITY_TEST_FILEIO_H

#include <fstream>
#include <string>
#include <vector>
#include <numeric>

class BadFileException : public std::exception {
    const char * what () const noexcept {
        return "Error while working with given file";
    }
};

// reads given file by word and saves them to vector
std::vector<std::string> read_by_word(const std::string& filename);

// appends sum and average of the given vector and given time to file
void write_results(const std::string& filename,
                   const std::vector<double>& numbers,
                   long long time);


#endif //PRODUCTIVITY_TEST_FILEIO_H
