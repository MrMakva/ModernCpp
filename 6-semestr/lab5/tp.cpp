#include <cmath>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;

class ThreadPool {
public:
    ThreadPool(size_t num_threads) : stop(false)
    {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;

                    {
                        unique_lock<mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });

                        if (this->stop && this->tasks.empty())
                            return;

                        task = move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
            -> future<typename result_of<F(Args...)>::type>
    {
        using return_type = typename result_of<F(Args...)>::type;

        auto task = make_shared<packaged_task<return_type()>>(
                bind(forward<F>(f), forward<Args>(args)...));

        future<return_type> res = task->get_future();
        {
            unique_lock<mutex> lock(queue_mutex);

            if (stop)
                throw runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }

        condition.notify_one();
        return res;
    }

    ~ThreadPool()
    {
        {
            unique_lock<mutex> lock(queue_mutex);
            stop = true;
        }

        condition.notify_all();
        for (thread& worker : workers)
            worker.join();
    }

private:
    vector<thread> workers;
    queue<function<void()>> tasks;

    mutex queue_mutex;
    condition_variable condition;
    bool stop;
};

double calculate_pi_part(int start, int end, double step)
{
    double sum = 0.0;
    for (int i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <num_threads>" << endl;
        return 1;
    }

    const size_t num_threads = stoi(argv[1]);
    const int num_steps = 100000000;
    const double step = 1.0 / num_steps;

    ThreadPool pool(num_threads);
    vector<future<double>> futures;

    const int block_size = num_steps / num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        int start = i * block_size;
        int end = (i == num_threads - 1) ? num_steps : (i + 1) * block_size;
        futures.emplace_back(pool.enqueue(calculate_pi_part, start, end, step));
    }

    double pi = 0.0;
    for (auto& future : futures) {
        pi += future.get();
    }
    pi *= step;

    cout.precision(15);
    cout << "Calculated Pi: " << pi << endl;
    cout << "Actual Pi:     " << M_PI << endl;
    cout << "Difference:    " << abs(pi - M_PI) << endl;

    return 0;
}