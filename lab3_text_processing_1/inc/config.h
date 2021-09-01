//
// Created by Maksym Protsyk on 3/21/21.
//

#ifndef DICT_BUILDER_CONFIG_H
#define DICT_BUILDER_CONFIG_H

#include <string>
#include <fstream>
#include <exception>
#include <map>

/*
 * Reads config file and returns the struct
 * which contains collected values
 */
struct config_t {
    std::string index_directory_path = ".";
    std::string by_alphabet_path = "alphabet.txt";
    std::string by_count_path = "count.txt";
    size_t threads = 1;
};

config_t read_config(const std::string& path);

#endif //DICT_BUILDER_CONFIG_H
