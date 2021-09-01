//
// Created by Maksym Protsyk on 2/18/21.
//
#include "methods.h"

std::vector<double> stod_method(const std::vector<std::string>& words) {
    std::vector<double> numbers;
    double current;
    for (const std::string& word: words) {
        try {
            current = stod(word);
            numbers.push_back(current);
        }
        catch (std::invalid_argument& e) {
            continue;
        }
        catch (std::out_of_range& e) {
            continue;
        }
    }
    return numbers;
}

std::vector<double> lexical_cast_method(const std::vector<std::string>& words) {
    std::vector<double> numbers;
    double current;
    for (const std::string& word: words) {
        try {
            current = boost::lexical_cast<double>(word);
            numbers.push_back(current);
        }
        catch (boost::bad_lexical_cast& e) {
            continue;
        }
    }
    return numbers;
}


std::vector<double> stream_method(const std::vector<std::string>& words) {
    std::vector<double> numbers;
    double current;
    for (const std::string& word: words) {
        std::stringstream str_stream;
        str_stream << word;
        while (str_stream >> current) {
            numbers.push_back(current);
        }
    }
    return numbers;
}
