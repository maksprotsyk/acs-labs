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
#include <archive.h>
#include <archive_entry.h>


struct parameters_t {
    std::vector<std::string> text_file_formats;
    std::vector<std::string> archive_file_formats;
    size_t max_size;
};

typedef std::queue<std::pair<std::string, bool>> processing_queue_t;

/*
 * Reads all files in the given archive (we pass its content)
 * And saves their content to the queue
 * (Skips all file formats except for specified in text_file_formats)
 */
std::vector<std::string> read_archive(
    const std::string& content,
    const parameters_t& params);


/*
 * Reads given file and returns a string with its content
 */
std::string read_in_memory(const std::string& file_name);

/*
 * Reads all files in the given folder and returns a queue with their content
 */
processing_queue_t read_all(
    const std::string& path,
    const parameters_t& params
);

/*
 * Saves dictionary with words and their numbers to the given file
 */
void dict_to_file(const std::string& filename, const std::vector<std::pair<std::string, size_t>>& dict);

#endif //DICT_BUILDER_FILES_H
