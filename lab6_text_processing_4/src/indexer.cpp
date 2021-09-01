//
// Created by Maksym Protsyk on 4/24/21.
//
#include "indexer.h"


config_t indexer_t::config;

std::vector<map_t> indexer_t::operator()(const indexing_pair_t& file_pair) const {
    if (file_pair.second) {
        return index_archive_files(file_pair.first);
    }
    return std::vector<map_t>{index_file(file_pair.first)};
}

map_t indexer_t::index_file(const std::string& text) {
    map_t local_dict{};
    blb::ssegment_index map(blb::word , text.begin(), text.end());
    map.rule(blb::word_any);
    for(blb::ssegment_index::iterator itr = map.begin(),end = map.end(); itr!=end; itr++){
        local_dict[boost::locale::fold_case(boost::locale::normalize(itr->str()))]++;
    }
    return local_dict;
}

std::vector<map_t> indexer_t::index_archive_files(const std::string& content) {
    std::vector<map_t> maps;
    for (const auto& text: unarchive(content)) {
        maps.emplace_back(index_file(text));
    }
    return maps;
}

std::vector<std::string> indexer_t::unarchive(const std::string& content) {
    struct archive* archive_ptr =  archive_read_new();
    struct archive_entry* entry_ptr = nullptr;
    archive_read_support_format_zip(archive_ptr);
    archive_read_support_filter_all(archive_ptr);
    std::vector<std::string> files{};

    if (archive_read_open_memory(archive_ptr, &content[0], content.size()) != ARCHIVE_OK) {
        throw std::runtime_error{"Can't open archive file"};
    }
    try {
        while (true) {
            auto res = archive_read_next_header(archive_ptr, &entry_ptr);
            if (res == ARCHIVE_EOF) {
                archive_read_close(archive_ptr);
                archive_read_free(archive_ptr);
                return files;
            }
            if (res != ARCHIVE_OK) {
                throw std::runtime_error{"Can't read next header"};
            }

            if (!archive_entry_size_is_set(entry_ptr)) {
                throw std::runtime_error{"Size of entry is not set"};
            }
            size_t file_size = archive_entry_size(entry_ptr);
            std::string name = std::filesystem::path(archive_entry_pathname(entry_ptr)).extension();

            if (file_size < config.max_size &&
                std::find(
                    config.text_file_formats.begin(),
                    config.text_file_formats.end(),
                    boost::locale::to_lower(name)) != config.text_file_formats.end()) {

                std::string str(file_size, char{});
                if (archive_read_data(archive_ptr, &str[0], file_size) != file_size) {
                    throw std::runtime_error{"Skipped entry"};
                }
                files.emplace_back(std::move(str));
            } else {
                archive_read_data_skip(archive_ptr);
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        archive_read_close(archive_ptr);
        archive_read_free(archive_ptr);
        return files;
    }
}