//
// Created by Maksym Protsyk on 3/20/21.
//

#ifndef DICT_BUILDER_PROCESSING_H
#define DICT_BUILDER_PROCESSING_H

#include <queue>
#include <string>
#include <map>
#include <utility>
#include <thread>
#include <mutex>
#include <boost/locale.hpp>
#include "config.h"
#include "time_measurements.h"
#include "files.h"


// files of greater size are skipped (in bytes)
#define MAX_FILE_SIZE 10000000

namespace blb = boost::locale::boundary;

// for testing different types of maps
typedef std::map<std::string, size_t> map_t;

/*
 * Adds the counted occurrences of words in one dictionary to another
 */
void merge_dicts(map_t& all_items, const map_t& new_items);

/*
 * Counts occurrences of words in string and add the results to the global dictionary
 * (mutex - for locking when add items to the global dictionary)
 */
void process_item(const std::string& text, map_t& global_dict, std::mutex& merge_lock);

/*
 * Counts occurrences of words in every text file in archive and add the results to the global dictionary
 * (mutex - for locking when add items to the global dictionary)
 */
void process_archive(
    const std::string& content, map_t& global_dict,
    std::mutex& merge_lock, const parameters_t& params
);

/*
 * Processes (as items with process_item or
 * process_archive depending on the second entry in pair)
 * all items in the queue (thread-safe)
 */
void process_queue(
    processing_queue_t& items,
    map_t& global_dict,
    std::mutex& queue_lock,
    std::mutex& merge_lock,
    const parameters_t& params
);


/*
 * Processes (as in "process_item" function) all items in the
 * queue using given number of threads.
 * Returns the dictionary with words and number of their occurrences
 */
map_t queue_multi_processing(processing_queue_t& items, size_t thread_num, const parameters_t& params);


/*
 * Converts map to vector of pairs
 * (Without copying)
 */
std::vector<std::pair<std::string, size_t>> map_to_vector(map_t& dict);


void set_locale(const std::string& id );

struct measurements_t {
    size_t total;
    size_t reading;
    size_t indexing;
};

/*
 * Builds and saves dictionaries for the given config
 * Returns time spend on reading, indexing and total time
 */
measurements_t index_and_measure_time(const config_t& config);

#endif //DICT_BUILDER_PROCESSING_H
