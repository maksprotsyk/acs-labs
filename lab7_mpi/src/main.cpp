#include <boost/mpi.hpp>
#include <iostream>
#include <boost/program_options.hpp>
#include "temperatures.h"
#include "mpi_modules.h"


namespace po = boost::program_options;


int main(int argc, char *argv[]) {
    boost::mpi::environment env{argc, argv};
    boost::mpi::communicator world;
    std::string config;
    if (argc == 1) {
        config = "config.dat";
    } else {
        config = argv[1];
    }


    po::options_description desc("Params");
    desc.add_options()
        ("dx", po::value<double>()->default_value(0.01))
        ("dy", po::value<double>()->default_value(0.01))
        ("alpha", po::value<double>()->default_value(0.01))
        ("dt", po::value<double>()->default_value(0.001))
        ("iterations", po::value<size_t>()->default_value(1000))
        ("iter_per_save", po::value<size_t>()->default_value(100))
        ("input_file", po::value<std::string>())
        ("output_file", po::value<std::string>())
        ;

    po::variables_map vm;
    po::store(po::parse_config_file(config.c_str(), desc), vm);
    po::notify(vm);


    temp_params_t params{};
    params.dx = vm["dx"].as<double>();
    params.dy = vm["dy"].as<double>();
    params.dt = vm["dt"].as<double>();
    params.alpha = vm["alpha"].as<double>();
    params.iterations = vm["iterations"].as<size_t>();
    params.iterations_per_save = vm["iter_per_save"].as<size_t>();
    if (vm.count("input_file") && vm.count("output_file")) {
        params.input_file = vm["input_file"].as<std::string>();
        params.output_file = vm["output_file"].as<std::string>();
    }
    else {
        throw std::runtime_error("Can't read input or output file from config");
    }



    if (world.rank() == world.size() - 1) {
        io_thread(world, params);
    } else {
        processing_thread(world, params);
    }



    return 0;
}