//
// Created by Maksym Protsyk on 3/3/21.
//
#include <integrals.h>

void calculate_integral(const integral_parameters_t& params,
                        size_t index, double* result, std::mutex& mut) {
    if (params.step.first <= 0 || params.step.second <= 0) {
        throw std::invalid_argument("Steps must be > 0");
    }
    if (!result) {
        throw std::invalid_argument("Passed a null pointer");
    }
    if (index >= params.points1.size() || index >= params.points2.size()) {
        throw std::invalid_argument("The index is too big");
    }
    // we are calculating values of function in the centers of rectangles
    // for better accuracy
    double x_start = params.points1[index].first + params.step.first / 2;
    double x = x_start;
    double y = params.points1[index].second + params.step.second / 2;
    double volume = 0;

    // adding step/4 to get rid of the error when adding many small doubles
    double last_y = params.points2[index].second + params.step.second / 4;
    double last_x = params.points2[index].first + params.step.first / 4;

    while (y < last_y) {
        while (x < last_x) {
            volume += params.func(x, y);
            x += params.step.first;
        }
        x = x_start;
        y += params.step.second;
    }
    mut.lock();
    *result += volume * params.step.first * params.step.second;
    mut.unlock();
}


double perform_integral_iteration(const integral_parameters_t& params) {
    std::vector<std::thread> threads{};
    std::mutex mut;
    double result = 0;
    for (size_t i = 1; i < params.points1.size(); i++) {
        threads.emplace_back(calculate_integral,
                             std::cref(params),
                             i, &result, std::ref(mut));
    }
    calculate_integral(params, 0, &result, mut);

    for (size_t i = 0; i < params.points1.size() - 1; i++){
        threads[i].join();
    }

    return result;

}



integral_results_t complete_integral_calculation(
    const program_parameters_t& prog_params,
    const std::function<double(double, double)>& func,
    size_t steps_num) {
    integral_parameters_t integral_params{};

    // special case when integral is equal to zero
    if (prog_params.y2 == prog_params.y1 || prog_params.x2 == prog_params.x1) {
        return integral_results_t{0, 0, 0};
    }

    // choosing integration limits for each thread
    double thread_step = (prog_params.y2 - prog_params.y1) / prog_params.threads;
    double current_y = prog_params.y1;
    for (size_t i = 0; i < prog_params.threads; i++) {
        integral_params.points1.emplace_back(prog_params.x1, current_y);
        current_y += thread_step;
        integral_params.points2.emplace_back(prog_params.x2, current_y);
    }

    // integer division, so that every thread works with the same amount of values
    size_t steps_per_thread = steps_num / prog_params.threads;

    integral_params.step = std::make_pair(
        (prog_params.x2 - prog_params.x1) / steps_num,
        thread_step/steps_per_thread
    );
    integral_params.func = func;

    // base integral calculation
    double prev_result = perform_integral_iteration(integral_params);
    double absolute_error = prev_result;
    double relative_error = 1;

    for (size_t i = 0; i < prog_params.max_iterations; i++) {
        // decreasing step to achieve better accuracy
        integral_params.step.first /= 2;
        integral_params.step.second /=2;

        double new_result = perform_integral_iteration(integral_params);


        // checking error values and saving new result
        absolute_error = std::abs(new_result - prev_result);
        relative_error = absolute_error/ std::abs(new_result);
        prev_result = new_result;
        if ((absolute_error < prog_params.absolute_error)
            && (relative_error < prog_params.relative_error)) {
            break;
        }
    }

    integral_results_t results{prev_result, absolute_error, relative_error};
    return results;
}


void read_and_check(std::string& str, std::ifstream& file) {
    if (!(file >> str)) {
        throw std::logic_error("Bad configuration file");
    }
}


program_parameters_t read_configuration(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::logic_error("Can't open configuration file");
    }
    program_parameters_t params{};

    std::string arg;
    read_and_check(arg, file);
    params.x1 = std::stod(arg);
    read_and_check(arg, file);
    params.x2 = std::stod(arg);
    if (params.x2 <= params.x1) {
        throw std::logic_error("Invalid x range");
    }

    read_and_check(arg, file);
    params.y1 = std::stod(arg);
    read_and_check(arg, file);
    params.y2 = std::stod(arg);
    if (params.y2 <= params.y1) {
        throw std::logic_error("Invalid y range");
    }

    read_and_check(arg, file);
    params.absolute_error = std::stod(arg);
    read_and_check(arg, file);
    params.relative_error = std::stod(arg);

    read_and_check(arg, file);
    params.threads = std::stoul(arg);
    file.close();
    return params;
}


void write_calculation_results(
    const integral_results_t& res, const std::string& filename
) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        throw std::logic_error("Can't open output file");
    }

    file << res.value << " "
         << res.absolute_error << " "
         << res.relative_error << " "
         << res.time << std::endl;
    file.close();
}
