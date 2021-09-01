//
// Created by Maksym Protsyk on 5/21/21.
//

#include "mpi_modules.h"


void io_thread(const boost::mpi::communicator& world, const temp_params_t& params) {

    // reading initial array
    TwoDimArray array(params.input_file);

    size_t width = array.width();
    size_t height = array.height();
    size_t process_num = world.size() - 1;

    // splitting rows between workers
    size_t per_worker = height / process_num;
    size_t additional_height = height % process_num;

    auto* input_data_requests = new boost::mpi::request[3 * process_num];

    size_t current_row = 0;

    // height is not always divisible by process_num so some workers will have
    // one additional row
    size_t elems_per_worker = width * (per_worker + 1);


    // sending 3 requsts for every worker: number of rows, array width, elements
    for (size_t i = 0; i < additional_height; i++) {
        input_data_requests[3 * i] = world.isend(i, height_data, per_worker + 1);
        input_data_requests[3 * i + 1] = world.isend(i, width_data, width);
        input_data_requests[3 * i + 2] = world.isend(i, rows_data, array[current_row], elems_per_worker);
        current_row += per_worker + 1;
    }

    // workers without additional rows
    elems_per_worker = width * per_worker;

    for (size_t i = additional_height; i < process_num; i++) {
        input_data_requests[3 * i] = world.isend(i, height_data, per_worker);
        input_data_requests[3 * i + 1] = world.isend(i, width_data, width);
        input_data_requests[3 * i + 2] = world.isend(i, rows_data, array[current_row], elems_per_worker);
        current_row += per_worker;
    }


    // waiting till data is sent to all workers
    boost::mpi::wait_all(input_data_requests, input_data_requests + 3 * process_num);

    delete[] input_data_requests;

    // writing initial array to the output file
    std::ofstream output(params.output_file);
    output << array << std::endl;

    // number of total saved array states
    size_t saves_num = (params.iterations / params.iterations_per_save);



    for (size_t i = 0; i < saves_num; i += 1) {
        auto* save_requests = new boost::mpi::request[process_num];
        current_row = 0;
        elems_per_worker = width * (per_worker + 1);

        // sending request to every worker to get their current data
        for (size_t j = 0; j < additional_height; j++) {
            save_requests[j] = world.irecv(j, save_data, array[current_row], elems_per_worker);
            current_row += per_worker + 1;
        }

        elems_per_worker = width * per_worker;

        for (size_t j = additional_height; j < process_num; j++) {
            save_requests[j] = world.irecv(j, save_data, array[current_row], elems_per_worker);
            current_row += per_worker;
        }

        boost::mpi::wait_all(save_requests, save_requests + process_num);

        delete[] save_requests;

        // writing to the output
        output << "\n" << array << std::endl;;

    }




}



void processing_thread(const boost::mpi::communicator& world, const temp_params_t& params) {
    // initialising height and width variables
    size_t height = 0;
    size_t width = 0;

    // getting number of rows and columns in our array
    world.recv(world.size() - 1, height_data, height);
    world.recv(world.size() - 1, width_data, width);
    size_t start_row = 1;
    size_t height_with_borders;


    if (world.size() == 2) {
        // if there is only one worker then it has constant borders
        height_with_borders = height;
        start_row = 0;
    }
    else if (world.rank() == 0) {
        // if it is the first worker it has constant first row
        height_with_borders = height + 1;
        start_row = 0;
    }
    else if (world.rank() == world.size() - 2) {
        // if it is the last worker it has constant last row
        height_with_borders = height + 1;
    }
    else {
        // all other rows need to get first and last row from other workers
        height_with_borders = height + 2;
    }

    TwoDimArray array(height_with_borders, width, 0);

    // getting data from io worker
    world.recv(world.size() - 1, rows_data, array[start_row], height * width);

    // new array for conaining new temperatures on each iterations
    TwoDimArray new_array(height_with_borders, width);

    // initialising left and right borders
    for (size_t i = 1; i < array.height() - 1; i++) {
        new_array.get(0, i) = array.get(0, i);
        new_array.get(array.width() - 1, i) = array.get(array.width() - 1, i);
    }

    // copying first row if it is the first worker
    if (world.rank() == 0) {
        memcpy(new_array[0], array[0], sizeof(double) * array.width());
    }

    // copying ladt row if it is the last worker
    if (world.rank() == world.size() - 2) {
        memcpy(new_array[array.height() -1], array[array.height() - 1], sizeof(double) * array.width());
    }



    TwoDimArray* array_ptr = &array;
    TwoDimArray* new_array_ptr = &new_array;

    // variable for containing request to save data to io worker
    boost::mpi::request save_request;
    bool saving = false;

    // our last iteration should save data
    // (there is no need to make more iterations if the output won't get to the file)
    size_t iterations = params.iterations - params.iterations % params.iterations_per_save;

    size_t i = 0;
    while (i < iterations) {

        if (world.size() != 2) {
            // sending request to get new borders
            // for different workers we may need different number of borders
            size_t requests_num = 0;
            auto requests = new boost::mpi::request[4];
            if (world.rank() != 0) {
                requests[requests_num] = world.isend(world.rank() - 1, upper_row, (*array_ptr)[1], width);
                requests[requests_num + 1] = world.irecv(world.rank() - 1, lower_row, (*array_ptr)[0], width);
                requests_num += 2;
            }
            if (world.rank() != world.size() - 2) {
                requests[requests_num] = world.isend(
                    world.rank() + 1,
                    lower_row,
                    (*array_ptr)[height_with_borders - 2],
                    width);
                requests[requests_num + 1] = world.irecv(
                    world.rank() + 1,
                    upper_row,
                    (*array_ptr)[height_with_borders - 1],
                    width
                );
                requests_num += 2;
            }

            boost::mpi::wait_all(requests, requests + requests_num);

            delete[] requests;
        }


        calculate_temperature(array_ptr, new_array_ptr, params);

        // we can't exchange pointers until safe request is ended
        if (saving) {
            save_request.wait();
            saving = false;
        }

        // exchanging states
        std::swap(array_ptr, new_array_ptr);

        if (i % params.iterations_per_save == params.iterations_per_save - 1) {
            // sending request to io worker
            save_request = world.isend(world.size() - 1, save_data, array[start_row], height * width);
            saving = true;
        }


        i++;
    }

    // if there is a saving request, we need to wait until it is finished
    if (saving) {
        save_request.wait();
    }



}
