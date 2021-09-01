//
// Created by Maksym Protsyk on 3/20/21.
//

#include "processing.h"



global_dict_t library_multi_processing(const config_t& config, measurements_t& measurements) {
    recursive_reader_t reader;
    indexer_t indexer;
    merger_t merger;
    recursive_reader_t::config = config;
    recursive_reader_t::itr = std::filesystem::recursive_directory_iterator(config.index_directory_path);
    recursive_reader_t::end = std::filesystem::end(recursive_reader_t::itr);
    indexer_t::config = config;
    merger_t::config = config;

    auto start = get_current_time_fenced();
    tbb::filter_t<void, indexing_pair_t> read_filter(tbb::filter::serial_in_order, reader);
    tbb::filter_t<indexing_pair_t, std::vector<map_t>> index_filter(tbb::filter::parallel, indexer);
    tbb::filter_t<std::vector<map_t>, void> merge_filter(tbb::filter::parallel, merger);

    tbb::filter_t<void,void> complete_filter = read_filter & index_filter & merge_filter;
    tbb::parallel_pipeline(config.tokens_num, complete_filter);
    auto finish = get_current_time_fenced();
    measurements.indexing = to_us(finish - start);

    return merger.global_dict;
}

std::vector<std::pair<std::string, size_t>> map_to_vector(global_dict_t& dict) {
    std::vector<std::pair<std::string, size_t>> pairs{};
    for (auto& entry: dict) {
        pairs.emplace_back(std::move(entry));
    }
    return pairs;
}


void set_locale(const std::string& id ) {
    boost::locale::generator generator;
    std::locale::global(generator(id));
}

measurements_t index_and_measure_time(const config_t& config) {
    auto start = get_current_time_fenced();
    measurements_t results{};
    set_locale("en_US.UTF-8");


    std::filesystem::path index_directory{config.index_directory_path};
    if (!std::filesystem::exists(index_directory) || !std::filesystem::is_directory(index_directory)) {
        throw std::runtime_error("Invalid indexing directory");
    }
    global_dict_t global_dict = library_multi_processing(config, results);

    std::vector<std::pair<std::string, size_t>> pairs = map_to_vector(global_dict);
    std::sort(pairs.begin(), pairs.end(), [](const auto& x, const auto& y){
        return x.first < y.first;
    });

    dict_to_file(config.by_alphabet_path, pairs);
    std::sort(pairs.begin(), pairs.end(), [](const auto& x, const auto& y){
        if (x.second > y.second) {
            return true;
        }
        else if (x.second == y.second) {
            return x.first < y.first;
        }
        return false;
    });
    dict_to_file(config.by_count_path, pairs);
    auto finish = get_current_time_fenced();
    results.total = to_us(finish - start);
    return results;
}

void dict_to_file(const std::string& filename, const std::vector<std::pair<std::string, size_t>>& dict) {
    std::ofstream file(filename, std::ofstream::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open output file");
    }
    for (const auto& entry: dict) {
        file << entry.first << ": " << entry.second << "\n";
    }
}
