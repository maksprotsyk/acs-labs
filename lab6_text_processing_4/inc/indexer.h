//
// Created by Maksym Protsyk on 4/24/21.
//

#ifndef DICT_BUILDER_INDEXER_H
#define DICT_BUILDER_INDEXER_H

#include "config.h"
#include "types.h"
#include <string>
#include <boost/locale.hpp>
#include <archive.h>
#include <archive_entry.h>



namespace blb = boost::locale::boundary;


class indexer_t {
public:
    static config_t config;

    std::vector<map_t> operator()(const indexing_pair_t& file_pair) const;

    static map_t index_file(const std::string& text);

    static std::vector<map_t> index_archive_files(const std::string& content);

    static std::vector<std::string> unarchive(const std::string& content);
};




#endif //DICT_BUILDER_INDEXER_H
