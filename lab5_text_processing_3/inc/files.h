//
// Created by Maksym Protsyk on 3/18/21.
//

#ifndef DICT_BUILDER_FILES_H
#define DICT_BUILDER_FILES_H

#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <filesystem>
#include <queue>
#include <boost/locale.hpp>
#include <boost/lockfree/queue.hpp>
#include <tbb/concurrent_queue.h>
#include <archive.h>
#include <archive_entry.h>
#include "safe_queue.h"
#include "config.h"


typedef std::shared_ptr<std::string> str_ptr;
typedef tbb::concurrent_bounded_queue<std::pair<str_ptr, bool>> indexing_queue_t;


// reads files in archive and saves pointers to them into archive
std::vector<str_ptr> read_archive(
    const std::string& content,
    const config_t& config
);


// reads file in memory and returns pointer to its content
str_ptr read_in_memory(const std::string& file_name);

// recursively iterates over files and send them to indexing queue
void read_all(
    indexing_queue_t& files,
    const config_t& config
);

// saves given dictionary to file
void dict_to_file(const std::string& filename, const std::vector<std::pair<std::string, size_t>>& dict);

#endif //DICT_BUILDER_FILES_H
