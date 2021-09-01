//
// Created by Maksym Protsyk on 4/24/21.
//

#ifndef DICT_BUILDER_TYPES_H
#define DICT_BUILDER_TYPES_H

#include <map>
#include <string>

typedef std::map<std::string, size_t> map_t;
typedef std::pair<std::string, bool> indexing_pair_t;

struct measurements_t {
    size_t indexing;
    size_t total;
};

#endif //DICT_BUILDER_TYPES_H
