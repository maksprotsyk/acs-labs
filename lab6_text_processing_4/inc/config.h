//
// Created by Maksym Protsyk on 3/21/21.
//

#ifndef DICT_BUILDER_CONFIG_H
#define DICT_BUILDER_CONFIG_H

#include <string>
#include <fstream>
#include <exception>
#include <map>
#include <vector>


struct config_t {
    std::string index_directory_path;
    std::string by_alphabet_path;
    std::string by_count_path;
    std::vector<std::string> text_file_formats;
    std::vector<std::string> archive_file_formats;
    size_t tokens_num;
    size_t max_size;
};

void read_config(config_t& config, const std::string& path);

#endif //DICT_BUILDER_CONFIG_H
