#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <sstream>
#include <thread>
#include <vector>
using namespace std;

template <typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), size_(0), head_(0), tail_(0)
    {
    }

    void push(T item)
    {
        auto start = chrono::steady_clock::now();

        unique_lock<mutex> lock(mutex_);
        not_full_.wait(
                lock, [this]() { return size_ < capacity_ || !running_; });
        if (!running_)
            return;

        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
        ++size_;

        auto end = chrono::steady_clock::now();
        total_push_wait_time_
                += chrono::duration_cast<chrono::milliseconds>(
                           end - start)
                           .count();
        push_count_++;

        not_empty_.notify_one();
    }

    T pop()
    {
        auto start = chrono::steady_clock::now();

        unique_lock<mutex> lock(mutex_);
        not_empty_.wait(lock, [this]() { return size_ > 0 || !running_; });
        if (!running_)
            return T();

        T item = buffer_[head_];
        head_ = (head_ + 1) % capacity_;
        --size_;

        auto end = chrono::steady_clock::now();
        total_pop_wait_time_
                += chrono::duration_cast<chrono::milliseconds>(
                           end - start)
                           .count();
        pop_count_++;

        not_full_.notify_one();
        return item;
    }

    size_t size() const
    {
        lock_guard<mutex> lock(mutex_);
        return size_;
    }

    double average_push_wait_time() const
    {
        return push_count_ == 0
                ? 0.0
                : static_cast<double>(total_push_wait_time_) / push_count_;
    }

    double average_pop_wait_time() const
    {
        return pop_count_ == 0
                ? 0.0
                : static_cast<double>(total_pop_wait_time_) / pop_count_;
    }

    void stop()
    {
        lock_guard<mutex> lock(mutex_);
        running_ = false;
        not_full_.notify_all();
        not_empty_.notify_all();
    }

private:
    vector<T> buffer_;
    size_t capacity_;
    size_t size_;
    size_t head_;
    size_t tail_;

    mutable mutex mutex_;
    condition_variable not_full_;
    condition_variable not_empty_;

    atomic<long long> total_push_wait_time_{0};
    atomic<long long> push_count_{0};
    atomic<long long> total_pop_wait_time_{0};
    atomic<long long> pop_count_{0};
    atomic<bool> running_{true};
};

class Logger {
public:
    static void log(const string& message)
    {
        lock_guard<mutex> lock(mutex_);
        cout << message << endl;
    }

private:
    static mutex mutex_;
};

mutex Logger::mutex_;

void publisher_thread(
        CircularBuffer<int>& buffer,
        double mean,
        double stddev,
        atomic<bool>& running)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> dist(mean, stddev);

    while (running) {
        int n = static_cast<int>(dist(gen));
        if (n < 0)
            n = 0;

        buffer.push(n);
        this_thread::sleep_for(chrono::milliseconds(n));
    }
}

void subscriber_thread(
        CircularBuffer<int>& buffer,
        double mean,
        double stddev,
        atomic<bool>& running)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> dist(mean, stddev);

    while (running) {
        int s = static_cast<int>(dist(gen));
        if (s < 0)
            s = 0;

        int n = buffer.pop();
        if (!running)
            break;

        size_t current_size = buffer.size();

        ostringstream oss;
        oss << "Получено: " << n << ", Размер буфера: " << current_size
            << ", Среднее время ожидания push: " << fixed
            << setprecision(2) << buffer.average_push_wait_time() << "ms";
        Logger::log(oss.str());

        this_thread::sleep_for(chrono::milliseconds(s));
    }
}

int main(int argc, char* argv[])
{
    if (argc != 6) {
        cerr
                << "Использование: " << argv[0]
                << " <buf_cap> <pub_mean> <pub_stddev> <sub_mean> <sub_stddev>"
                << endl;
        return 1;
    }

    size_t buf_cap = stoul(argv[1]);
    double pub_mean = stod(argv[2]);
    double pub_stddev = stod(argv[3]);
    double sub_mean = stod(argv[4]);
    double sub_stddev = stod(argv[5]);

    CircularBuffer<int> buffer(buf_cap);
    atomic<bool> running{true};

    thread publisher(
            publisher_thread,
            ref(buffer),
            pub_mean,
            pub_stddev,
            ref(running));
    thread subscriber(
            subscriber_thread,
            ref(buffer),
            sub_mean,
            sub_stddev,
            ref(running));

    this_thread::sleep_for(chrono::seconds(30));

    running = false;
    buffer.stop();

    publisher.join();
    subscriber.join();

    cout << "\nИтоговая статистика:\n";
    cout << "Среднее время ожидания push: " << fixed
              << setprecision(2) << buffer.average_push_wait_time()
              << "ms\n";
    cout << "Среднее время ожидания pop: " << fixed
              << setprecision(2) << buffer.average_pop_wait_time()
              << "ms\n";

    return 0;
}