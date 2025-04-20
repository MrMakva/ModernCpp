#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
using namespace std;

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> future<typename result_of<F(Args...)>::type>;
    
    ~ThreadPool();

private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    
    mutex queue_mutex;
    condition_variable condition;
    bool stop;
};

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;
    
    auto task = make_shared<packaged_task<return_type()>>(
        bind(forward<F>(f), forward<Args>(args)...)
    );
        
    future<return_type> res = task->get_future();
    {
        unique_lock<mutex> lock(queue_mutex);
        
        if(stop)
            throw runtime_error("enqueue on stopped ThreadPool");
            
        tasks.emplace([task](){ (*task)(); });
    }
    
    condition.notify_one();
    return res;
}

#endif
