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
#include <memory>
#include "config.h"
#include "time_measurements.h"
#include "files.h"
#include "safe_queue.h"


namespace blb = boost::locale::boundary;


// for testing different types of maps
typedef std::map<std::string, size_t> map_t;


typedef safe_queue_t<std::shared_ptr<map_t>> merging_queue_t;

// time measurement results
struct measurements_t {
    size_t io;
    size_t indexing;
    size_t total;
};

// add values of the second dictionary in the first one
void merge_dicts(map_t& all_items, const map_t& new_items);


// processes archive file and sends respective dictionaries to the merging queue
void process_archive(
    merging_queue_t& queue,
    const std::string& content, const config_t& config
);


// create a dict for the given text file and sends it to the merging queue
void index_and_send(merging_queue_t& queue, const std::string& text);


// processes items from the files queue and sends them to the merging queue
void process_index_queue_items(
    indexing_queue_t& in_queue, merging_queue_t& out_queue, const config_t& config
);


// processes items from the files queue and sends them to the merging queue
void process_merge_queue_items(merging_queue_t& out_queue, map_t& dict);

// processes items from the files queue and sends them to the merging queue
// if there are no more files, then starts merging
void index_and_merge_thread(indexing_queue_t& in_queue, merging_queue_t& out_queue, const config_t& config, map_t& dict);


// processes the library consisting of archives and text files using given config
// returns dictionary of all words, saves time measurements results into measurement variable
map_t library_multi_processing(const config_t& config, measurements_t& measurements);


// converts dictionary map to vector
std::vector<std::pair<std::string, size_t>> map_to_vector(map_t& dict);


void set_locale(const std::string& id);


// full program for the given config
measurements_t index_and_measure_time(const config_t& config);

#endif //DICT_BUILDER_PROCESSING_H
