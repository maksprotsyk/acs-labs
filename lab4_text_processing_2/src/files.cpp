//
// Created by Maksym Protsyk on 3/18/21.
//
#include <files.h>


std::vector<str_ptr> read_archive(
    const std::string& content,
    const config_t& config
) {

    struct archive* archive_ptr =  archive_read_new();
    struct archive_entry* entry_ptr = nullptr;
    archive_read_support_format_zip(archive_ptr);
    archive_read_support_filter_all(archive_ptr);
    std::vector<std::shared_ptr<std::string>> files{};

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
                files.push_back(std::make_shared<std::string>(str));
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


str_ptr read_in_memory(const std::string& file_name) {
    std::ifstream raw_file(file_name, std::ios::binary);
    std::ostringstream buffer_ss;
    buffer_ss << raw_file.rdbuf();
    std::string buffer{buffer_ss.str()};
    return std::make_shared<std::string>(buffer);
}


void read_all(
    indexing_queue_t& files,
    const config_t& config
) {
    for(auto& file: std::filesystem::recursive_directory_iterator(config.index_directory_path)) {
        //std::cout << file.path() << std::endl;
        try {
            if (file.is_regular_file()) {
                std::string file_extension = boost::locale::to_lower(file.path().extension().string());
                if (std::find(config.archive_file_formats.begin(),
                              config.archive_file_formats.end(),
                              file_extension)
                    != config.archive_file_formats.end()) {
                    files.emplace(read_in_memory(file.path()), true);

                }
                else if (std::find(config.text_file_formats.begin(),
                                   config.text_file_formats.end(),
                                   file_extension)
                         != config.text_file_formats.end() &&
                         file.file_size() <= config.max_size)
                {
                    files.emplace(read_in_memory(file.path()), false);
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << file.path() << std::endl;
        }

    }
    --files.producers;
    // telling waiting threads to check if there are any consumers left
    files.notify_finish();
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