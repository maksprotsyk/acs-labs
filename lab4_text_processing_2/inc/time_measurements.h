//
// Created by Maksym Protsyk on 3/21/21.
//

#ifndef DICT_BUILDER_TIME_MEASUREMENTS_H
#define DICT_BUILDER_TIME_MEASUREMENTS_H

#include <atomic>
#include <chrono>
#include <vector>

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

#endif //DICT_BUILDER_TIME_MEASUREMENTS_H
