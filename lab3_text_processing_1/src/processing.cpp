//
// Created by Maksym Protsyk on 3/20/21.
//

#include "processing.h"


void merge_dicts(map_t& all_items, const map_t& new_items) {
    for (const auto& dict_entry: new_items) {
        all_items[dict_entry.first] += dict_entry.second;
    }
}

void process_item(const std::string& text, map_t& global_dict, std::mutex& merge_lock) {
    map_t local_dict{};
    blb::ssegment_index map(blb::word , text.begin(), text.end());
    map.rule(blb::word_any);
    for(blb::ssegment_index::iterator itr=map.begin(), end=map.end(); itr!=end; itr++){
        local_dict[boost::locale::fold_case(boost::locale::normalize(itr->str()))] += 1;
    }

    merge_lock.lock();
    merge_dicts(global_dict, local_dict);
    merge_lock.unlock();
}

void process_archive(
    const std::string& content, map_t& global_dict,
    std::mutex& merge_lock, const parameters_t& params
    ) {
    for (const std::string& text: read_archive(content, params)) {
        process_item(text, global_dict, merge_lock);
    }
}


void process_queue(
    processing_queue_t& items,
    map_t& global_dict,
    std::mutex& queue_lock,
    std::mutex& merge_lock,
    const parameters_t& params
) {
    while (true) {
        queue_lock.lock();
        if (items.empty()) {
            queue_lock.unlock();
            return;
        }
        auto text = items.front();
        items.pop();
        queue_lock.unlock();
        if (text.second) {
            process_archive(text.first, global_dict, merge_lock, params);
        }
        else {
            process_item(text.first, global_dict, merge_lock);
        }
    }
}


map_t queue_multi_processing(processing_queue_t& items, size_t thread_num, const parameters_t& params) {
    map_t global_dict{};
    std::mutex queue_lock;
    std::mutex merge_lock;
    std::vector<std::thread> threads{};
    for (size_t i = 1; i < thread_num; i++) {
        threads.emplace_back(process_queue,
                             std::ref(items),
                             std::ref(global_dict),
                             std::ref(queue_lock),
                             std::ref(merge_lock),
                             std::cref(params));
    }
    process_queue(items, global_dict, queue_lock, merge_lock, params);
    for (size_t i = 1; i < thread_num; i++) {
        threads[i-1].join();
    }

    return global_dict;
}

std::vector<std::pair<std::string, size_t>> map_to_vector(map_t& dict) {
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
    parameters_t params{};
    params.archive_file_formats = {".zip"};
    params.text_file_formats = {".txt"};
    params.max_size = MAX_FILE_SIZE;

    std::filesystem::path index_directory{config.index_directory_path};
    if (!std::filesystem::exists(index_directory) || !std::filesystem::is_directory(index_directory)) {
        throw std::runtime_error("Invalid indexing directory");
    }
    auto reading_start = get_current_time_fenced();
    processing_queue_t files = read_all(config.index_directory_path, params);
    auto reading_finish = get_current_time_fenced();

    auto indexing_start = get_current_time_fenced();
    map_t global_dict = queue_multi_processing(files, config.threads, params);
    auto indexing_finish = get_current_time_fenced();

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

    results.reading = to_us(reading_finish - reading_start);
    results.indexing = to_us(indexing_finish - indexing_start);
    results.total = to_us(finish - start);
    return results;
}
