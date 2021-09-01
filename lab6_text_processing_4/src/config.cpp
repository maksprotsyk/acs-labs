//
// Created by Maksym Protsyk on 3/21/21.
//
#include "config.h"


void read_config(config_t& config, const std::string& path) {
    std::map<std::string, std::string> config_values{
        {"index_directory_path", config.index_directory_path},
        {"by_alphabet_path", config.by_alphabet_path},
        {"by_count_path", config.by_count_path},
        {"tokens_num", std::to_string(config.tokens_num)}
    };

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::logic_error{"bad file"};
    }
    std::string line;
    while (std::getline(file, line)) {
        size_t index = line.find('=');
        if (index == std::string::npos) {
            continue;
        }
        std::string key = line.substr(0, index);
        if (config_values.find(key) != config_values.end()) {
            config_values[key] = line.substr(index+1);
        }
    }
    config.index_directory_path = config_values["index_directory_path"];
    config.by_alphabet_path = config_values["by_alphabet_path"];
    config.by_count_path = config_values["by_count_path"];
    config.tokens_num = stoul(config_values["tokens_num"]);
    file.close();

}