//
// Created by Maksym Protsyk on 3/4/21.
//

#ifndef INTEGRAL_CALCULATOR_TIME_MEASUREMENTS_H
#define INTEGRAL_CALCULATOR_TIME_MEASUREMENTS_H

#include <atomic>
#include <chrono>
#include <vector>

// gets current time point
inline std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

#endif //INTEGRAL_CALCULATOR_TIME_MEASUREMENTS_H
