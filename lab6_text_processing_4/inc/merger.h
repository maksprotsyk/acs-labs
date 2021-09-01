//
// Created by Maksym Protsyk on 4/24/21.
//

#ifndef DICT_BUILDER_MERGER_H
#define DICT_BUILDER_MERGER_H

#include "config.h"
#include "indexer.h"
#include <tbb/concurrent_hash_map.h>

typedef tbb::concurrent_hash_map<std::string, size_t> global_dict_t;

class merger_t {
public:
    static config_t config;

    static global_dict_t global_dict;

    void operator()(const std::vector<map_t>& maps) const;
};





#endif //DICT_BUILDER_MERGER_H
