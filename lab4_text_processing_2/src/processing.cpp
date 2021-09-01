//
// Created by Maksym Protsyk on 3/20/21.
//

#include "processing.h"
#include <iostream>
#include <memory>


void merge_dicts(map_t& all_items, const map_t& new_items) {
    for (const auto& dict_entry: new_items) {
        all_items[dict_entry.first] += dict_entry.second;
    }

}

void index_and_send(merging_queue_t& queue, const std::string& text) {
    map_t local_dict{};
    blb::ssegment_index map(blb::word , text.begin(), text.end());
    map.rule(blb::word_any);
    for(blb::ssegment_index::iterator itr = map.begin(),end = map.end(); itr!=end; itr++){
        local_dict[boost::locale::fold_case(boost::locale::normalize(itr->str()))] += 1;
    }
    ++queue.counter;
    queue.emplace(std::make_shared<map_t>(local_dict));
}

void process_archive(
    merging_queue_t& queue,
    const std::string& content, const config_t& config
    ) {
    for (const auto& text: read_archive(content, config)) {
        index_and_send(queue, *text);
    }
}

void process_index_queue_items(
    indexing_queue_t& in_queue, merging_queue_t& out_queue, const config_t& config
    ) {
    try {
        while (true) {
            auto item = in_queue.pop();
            if (item.second) {
                process_archive(out_queue, *item.first, config);
            } else {
                index_and_send(out_queue, *item.first);
            }
            item.first.reset();
        }
    }
    catch (const std::runtime_error& e) {
        --in_queue.consumers;
        --out_queue.producers;
        // telling waiting threads to check if there are any producers or consumers left
        out_queue.notify_finish();
        in_queue.notify_finish();
    }
}

void process_merge_queue_items(merging_queue_t& out_queue, map_t& dict) {
    while (true) {
        auto item = out_queue.map_reduce_pop(2);
        if (item.size() == 2) {
            if (item[0]->size() >= item[1]->size()) {
                merge_dicts(*item[0], *item[1]);
                out_queue.push(item[0]);
                item[1].reset();
            } else {
                merge_dicts(*item[1], *item[0]);
                out_queue.push(item[1]);
                item[0].reset();
            }
        }
        else {
            --out_queue.consumers;
            if (item.size() == 1) {
                dict = *item[0];
                // telling waiting threads to check if there are any producers left
                out_queue.notify_finish();
            }

            break;
        }
    }
}

void index_and_merge_thread(
    indexing_queue_t& in_queue,
    merging_queue_t& out_queue,
    const config_t& config, map_t& dict) {
    process_index_queue_items(in_queue, out_queue, config);
    process_merge_queue_items(out_queue, dict);
}


map_t library_multi_processing(const config_t& config, measurements_t& measurements) {

    indexing_queue_t index_queue(
        config.index_queue_size,
        1,
        config.threads_indexing + 1
        );

    merging_queue_t merge_queue(
        config.merge_queue_size,
        config.threads_indexing + 1,
        config.threads_merging + config.threads_indexing + 1
        );

    map_t global_dict{};
    std::vector<std::thread> indexing{};
    std::vector<std::thread> merging{};

    for (size_t i = 0; i < config.threads_indexing; i++) {
        indexing.emplace_back(index_and_merge_thread,
                             std::ref(index_queue),
                             std::ref(merge_queue),
                             std::cref(config),
                             std::ref(global_dict));
    }
    for (size_t i = 0; i < config.threads_merging; i++) {
        merging.emplace_back(process_merge_queue_items,
                             std::ref(merge_queue),
                             std::ref(global_dict));
    }

    auto io_start = get_current_time_fenced();
    read_all(index_queue, config);
    auto io_finish = get_current_time_fenced();
    process_index_queue_items(index_queue, merge_queue, config);
    process_merge_queue_items(merge_queue, global_dict);

    for (size_t i = 0; i < config.threads_indexing; i++) {
        indexing[i].join();
    }
    for (size_t i = 0; i < config.threads_merging; i++) {
        merging[i].join();
    }
    auto indexing_finish = get_current_time_fenced();

    measurements.io = to_us(io_finish - io_start);
    measurements.indexing = to_us(indexing_finish - io_start);

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


    std::filesystem::path index_directory{config.index_directory_path};
    if (!std::filesystem::exists(index_directory) || !std::filesystem::is_directory(index_directory)) {
        throw std::runtime_error("Invalid indexing directory");
    }
    map_t global_dict = library_multi_processing(config, results);

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
