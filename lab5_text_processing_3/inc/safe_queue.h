//
// Created by Maksym Protsyk on 4/1/21.
//

#ifndef DICT_BUILDER_SAFE_QUEUE_H
#define DICT_BUILDER_SAFE_QUEUE_H
#include <queue>
#include <condition_variable>
#include <mutex>
#include <exception>
#include <atomic>


template <class T>
class safe_queue_t {
private:
    std::queue<T> queue{};
    std::condition_variable notifier;
    size_t max_size;
    std::mutex common_mutex;
    std::mutex pop_mutex;
public:
    // counter of items which theoretically should be in the queue
    // (used only for merging)
    std::atomic<int> counter = 0;
    std::atomic<int> producers;
    std::atomic<int> consumers;
    safe_queue_t(size_t size, int producers_num, int consumers_num) {
        max_size = size;
        producers = producers_num;
        consumers = consumers_num;
    };
    T pop();
    std::vector<T> map_reduce_pop(size_t n=2);
    template <class ...Args> void emplace(Args&&... args);
    void push(const T& item);
    size_t size();
    void notify_finish();
};


template <class T>
T safe_queue_t<T>::pop() {
    T item;
    std::unique_lock common_lock(common_mutex, std::defer_lock_t{});
    std::unique_lock pop_lock(pop_mutex, std::defer_lock_t{});
    {
        std::lock(common_lock, pop_lock);
        while (queue.size() == 0 && producers > 0) {
            notifier.wait(common_lock);
        }
        if (queue.size() == 0 and producers <= 0) {
            throw std::runtime_error{"queue finished working"};
        }
        item = queue.front();
        queue.pop();
    }
    notifier.notify_all();
    return item;

}

// pops n items at once, but notifies queue (decreases counter) that we have taken
// only n-1 items (we are expected to return one item which is result
// of some processes performed on the popped items)
template <class T>
std::vector<T> safe_queue_t<T>::map_reduce_pop(size_t n) {
    std::vector<T> items;
    std::unique_lock common_lock(common_mutex, std::defer_lock_t{});
    std::unique_lock pop_lock(pop_mutex, std::defer_lock_t{});
    {
        std::lock(common_lock, pop_lock);
        for (size_t i = 0; i < n; i++) {
            while (queue.size() == 0 && (producers > 0 || counter > 0)) {
                notifier.wait(common_lock);
            }
            if (queue.size() == 0) {
                return items;
            }
            counter -= 1;
            items.emplace_back(queue.front());
            queue.pop();
        }
        counter += 1;
    }
    notifier.notify_all();
    return items;

}


template <class T>
void safe_queue_t<T>::push(const T& item){
    {
        std::unique_lock common_lock(common_mutex);
        while (queue.size() >= max_size && consumers > 0) {
            notifier.wait(common_lock);
        }
        if (queue.size() >= max_size) {
            throw std::runtime_error{"queue is full but no consumers left"};
        }
        queue.push(item);
    }
    notifier.notify_all();

}

template <class T>
template <class ...Args> void safe_queue_t<T>::emplace(Args&&... args) {
    {
        std::unique_lock common_lock(common_mutex);

        while ((queue.size() >= max_size) && (consumers > 0)) {
            notifier.wait(common_lock);
        }

        if (queue.size() >= max_size) {
            throw std::runtime_error{"queue is full but no consumers left"};
        }
        queue.emplace(args...);
    }
    notifier.notify_all();
}

template <class T>
size_t safe_queue_t<T>::size() {
    std::lock_guard common_lock(common_mutex);
    return queue.size();
}

template <typename T>
void safe_queue_t<T>::notify_finish() {
    notifier.notify_all();
}



#endif //DICT_BUILDER_SAFE_QUEUE_H
