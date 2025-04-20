#include "tp.h"
#include <atomic>
#include <gtest/gtest.h>
using namespace std;

TEST(ThreadPoolTest, BasicFunctionality)
{
    ThreadPool pool(4);
    auto result = pool.enqueue([]() { return 42; });
    EXPECT_EQ(result.get(), 42);
}

TEST(ThreadPoolTest, MultipleTasks)
{
    ThreadPool pool(2);
    vector<future<int>> results;

    for (int i = 0; i < 10; ++i) {
        results.emplace_back(pool.enqueue([i]() { return i * i; }));
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(results[i].get(), i * i);
    }
}

TEST(ThreadPoolTest, ThreadSafety)
{
    const int num_tasks = 1000;
    ThreadPool pool(4);
    vector<future<int>> results;
    atomic<int> counter{0};

    for (int i = 0; i < num_tasks; ++i) {
        results.emplace_back(pool.enqueue([&counter]() { return counter++; }));
    }

    for (auto& result : results) {
        result.get();
    }

    EXPECT_EQ(counter.load(), num_tasks);
}

TEST(ThreadPoolTest, ExceptionHandling)
{
    ThreadPool pool(2);
    auto result = pool.enqueue([]() {
        throw runtime_error("Test exception");
        return 0;
    });

    EXPECT_THROW(result.get(), runtime_error);
}

TEST(ThreadPoolTest, DestructorWithPendingTasks)
{
    vector<future<void>> results;
    {
        ThreadPool pool(2);
        for (int i = 0; i < 10; ++i) {
            results.emplace_back(pool.enqueue([]() {
                this_thread::sleep_for(chrono::milliseconds(100));
            }));
        }
    }
    for (auto& result : results) {
        EXPECT_NO_THROW(result.get());
    }
}