//
// Created by Maksym Protsyk on 4/23/21.
//

#ifndef DICT_BUILDER_RECURSIVE_READER_H
#define DICT_BUILDER_RECURSIVE_READER_H
#include <string>
#include <filesystem>
#include "types.h"
#include "config.h"
#include <boost/locale.hpp>
#include <tbb/pipeline.h>


class recursive_reader_t {
public:
    static std::filesystem::recursive_directory_iterator itr;

    static std::filesystem::recursive_directory_iterator end;

    static config_t config;

    indexing_pair_t operator()(tbb::flow_control& fc) const;

    static std::string read_to_string(const std::string& file_name);
};


#endif //DICT_BUILDER_RECURSIVE_READER_H
