//
// Created by Maksym Protsyk on 4/24/21.
//

#include "merger.h"

config_t merger_t::config;
global_dict_t merger_t::global_dict;

void merger_t::operator()(const std::vector<map_t>& maps) const {
    global_dict_t::accessor writer;
    for (const auto& local_items: maps) {
        for (const auto& dict_entry: local_items) {
            global_dict.insert(writer, dict_entry.first);
            writer->second += dict_entry.second;
        }
    }
}