#ifndef SCB_H
#define SCB_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
using namespace std;

template <typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity);
    
    void push(T item);
    T pop();
    
    size_t size() const;
    bool empty() const;
    bool full() const;
    
    double average_push_wait_time() const;

private:
    vector<T> buffer_;
    size_t capacity_;
    size_t size_;
    size_t head_;
    size_t tail_;
    
    mutable mutex mutex_;
    condition_variable not_full_;
    condition_variable not_empty_;
    
    atomic<long long> total_push_wait_time_;
    atomic<long long> push_count_;
};

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t capacity) 
    : buffer_(capacity), capacity_(capacity), size_(0), head_(0), tail_(0),
      total_push_wait_time_(0), push_count_(0) {}

template <typename T>
void CircularBuffer<T>::push(T item) {
    unique_lock<mutex> lock(mutex_);
    not_full_.wait(lock, [this]() { return size_ < capacity_; });
    
    auto start = chrono::steady_clock::now();
    buffer_[tail_] = item;
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
    auto end = chrono::steady_clock::now();
    
    total_push_wait_time_ += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    push_count_++;
    not_empty_.notify_one();
}

template <typename T>
T CircularBuffer<T>::pop() {
    unique_lock<mutex> lock(mutex_);
    not_empty_.wait(lock, [this]() { return size_ > 0; });
    
    T item = buffer_[head_];
    head_ = (head_ + 1) % capacity_;
    --size_;
    
    not_full_.notify_one();
    return item;
}

template <typename T>
size_t CircularBuffer<T>::size() const {
    lock_guard<mutex> lock(mutex_);
    return size_;
}

template <typename T>
bool CircularBuffer<T>::empty() const {
    return size() == 0;
}

template <typename T>
bool CircularBuffer<T>::full() const {
    lock_guard<mutex> lock(mutex_);
    return size_ == capacity_;
}

template <typename T>
double CircularBuffer<T>::average_push_wait_time() const {
    return push_count_ == 0 ? 0.0 : static_cast<double>(total_push_wait_time_) / push_count_;
}

#endif
