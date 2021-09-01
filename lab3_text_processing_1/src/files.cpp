// Done!
// Created by Maksym Protsyk on 3/18/21.
//
#include <files.h>


std::vector<std::string> read_archive(
    const std::string& content,
    const parameters_t& params
    ) {

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

            if (file_size < params.max_size &&
                std::find(
                    params.text_file_formats.begin(),
                    params.text_file_formats.end(),
                    boost::locale::to_lower(name)) != params.text_file_formats.end()) {

                std::string str(file_size, char{});
                if (archive_read_data(archive_ptr, &str[0], file_size) != file_size) {
                    throw std::runtime_error{"Skipped entry"};
                }
                files.push_back(std::move(str));
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


std::string read_in_memory(const std::string& file_name) {
    std::ifstream raw_file(file_name, std::ios::binary);
    std::ostringstream buffer_ss;
    buffer_ss << raw_file.rdbuf();
    std::string buffer{buffer_ss.str()};
    raw_file.close();
    return buffer;
}


processing_queue_t read_all(
    const std::string& path,
    const parameters_t& params
    ) {
    processing_queue_t files;
    for(auto& file: std::filesystem::recursive_directory_iterator(path)) {
        try {
            if (file.is_regular_file()) {
                std::string file_extension = boost::locale::to_lower(file.path().extension().string());
                if (std::find(params.archive_file_formats.begin(),
                              params.archive_file_formats.end(),
                              file_extension) != params.archive_file_formats.end()) {
                    files.emplace(std::move(read_in_memory(file.path())), true);
                }
                else if (std::find(params.text_file_formats.begin(),
                                   params.text_file_formats.end(),
                                   file_extension)
                         != params.text_file_formats.end() &&
                         file.file_size() <= params.max_size)
                {
                    files.emplace(std::move(read_in_memory(file.path())), false);
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << file.path() << std::endl;
        }
    }
    return files;
}


void dict_to_file(const std::string& filename, const std::vector<std::pair<std::string, size_t>>& dict) {
    std::ofstream file(filename, std::ofstream::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open output file");
    }
    for (const auto& entry: dict) {
        file << entry.first << ": " << entry.second << "\n";
    }
    file.close();
}