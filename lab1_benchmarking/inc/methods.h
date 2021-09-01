//
// Created by Maksym Protsyk on 2/18/21.
//

#ifndef PRODUCTIVITY_TEST_METHODS_H
#define PRODUCTIVITY_TEST_METHODS_H

#include <vector>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>


// converts string vector to vector of doubles using stod function
std::vector<double> stod_method(const std::vector<std::string>& words);

// converts string vector to vector of doubles using boost lecixal cast
std::vector<double> lexical_cast_method(const std::vector<std::string>& words);

// converts string vector to vector of doubles using  stringstream
std::vector<double> stream_method(const std::vector<std::string>& words);

typedef std::vector<double> (*method_t)(const std::vector<std::string>& words);




#endif //PRODUCTIVITY_TEST_METHODS_H
