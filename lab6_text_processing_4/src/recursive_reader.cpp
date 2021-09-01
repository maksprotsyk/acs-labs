//
// Created by Maksym Protsyk on 4/23/21.
//

#include "recursive_reader.h"


std::filesystem::recursive_directory_iterator recursive_reader_t::itr;
std::filesystem::recursive_directory_iterator recursive_reader_t::end;
config_t recursive_reader_t::config;



indexing_pair_t recursive_reader_t::operator()(tbb::flow_control& fc) const {
    while (itr != end) {
        auto file = *itr;
        itr++;
        if (!file.is_regular_file()) {
            continue;
        }
        std::string file_extension = boost::locale::to_lower(file.path().extension().string());
        if (std::find(config.archive_file_formats.begin(),
                      config.archive_file_formats.end(),
                      file_extension)
            != config.archive_file_formats.end()) {
            return std::make_pair(read_to_string(file.path()), true);
        }
        if (std::find(config.text_file_formats.begin(),
                      config.text_file_formats.end(),
                      file_extension)
            != config.text_file_formats.end() &&
            file.file_size() <= config.max_size)
        {
            return std::make_pair(read_to_string(file.path()), false);
        }
    }
    fc.stop();
    return std::make_pair("", false);
}


std::string recursive_reader_t::read_to_string(const std::string& file_name) {
    std::ifstream raw_file(file_name, std::ios::binary);
    std::ostringstream buffer_ss;
    buffer_ss << raw_file.rdbuf();
    std::string buffer{buffer_ss.str()};
    return buffer;
}