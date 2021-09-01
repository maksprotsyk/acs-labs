//
// Created by Maksym Protsyk on 3/20/21.
//

#ifndef DICT_BUILDER_PROCESSING_H
#define DICT_BUILDER_PROCESSING_H

#include <string>
#include <boost/locale.hpp>
#include <tbb/pipeline.h>
#include "config.h"
#include "time_measurements.h"
#include "merger.h"
#include "recursive_reader.h"
#include "indexer.h"
#include "types.h"


// processes the library consisting of archives and text files using given config
// returns dictionary of all words, saves time measurements results into measurement variable
global_dict_t library_multi_processing(const config_t& config, measurements_t& measurements);


// converts dictionary map to vector
std::vector<std::pair<std::string, size_t>> map_to_vector(global_dict_t& dict);


void set_locale(const std::string& id);


// full program for the given config
measurements_t index_and_measure_time(const config_t& config);

void dict_to_file(const std::string& filename, const std::vector<std::pair<std::string, size_t>>& dict);

#endif //DICT_BUILDER_PROCESSING_H
