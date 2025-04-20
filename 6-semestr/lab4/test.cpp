#include "scb.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

class CircularBufferTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        buffer = new CircularBuffer<int>(5);
    }

    void TearDown() override
    {
        delete buffer;
    }

    CircularBuffer<int>* buffer;
};

TEST_F(CircularBufferTest, BasicOperations)
{
    EXPECT_EQ(buffer->size(), 0);
    EXPECT_TRUE(buffer->empty());
    EXPECT_FALSE(buffer->full());

    buffer->push(1);
    buffer->push(2);
    buffer->push(3);
    EXPECT_EQ(buffer->size(), 3);
    EXPECT_FALSE(buffer->empty());
    EXPECT_FALSE(buffer->full());

    EXPECT_EQ(buffer->pop(), 1);
    EXPECT_EQ(buffer->pop(), 2);
    EXPECT_EQ(buffer->size(), 1);

    buffer->push(4);
    buffer->push(5);
    buffer->push(6);
    EXPECT_EQ(buffer->size(), 4);
    EXPECT_FALSE(buffer->full());

    buffer->push(7);
    EXPECT_EQ(buffer->size(), 5);
    EXPECT_TRUE(buffer->full());

    EXPECT_EQ(buffer->pop(), 3);
    EXPECT_EQ(buffer->pop(), 4);
    EXPECT_EQ(buffer->pop(), 5);
    EXPECT_EQ(buffer->pop(), 6);
    EXPECT_EQ(buffer->pop(), 7);
    EXPECT_EQ(buffer->size(), 0);
    EXPECT_TRUE(buffer->empty());
}

TEST_F(CircularBufferTest, OverflowTest)
{
    for (int i = 0; i < 5; ++i) {
        buffer->push(i);
    }
    EXPECT_EQ(buffer->size(), 5);
    EXPECT_TRUE(buffer->full());

    bool running = true;
    bool item_pushed = false;
    thread producer([&]() {
        buffer->push(5);
        item_pushed = true;
        running = false;
    });

    this_thread::sleep_for(chrono::milliseconds(100));
    EXPECT_TRUE(running);
    EXPECT_FALSE(item_pushed);

    EXPECT_EQ(buffer->pop(), 0);

    producer.join();
    EXPECT_TRUE(item_pushed);
    EXPECT_EQ(buffer->size(), 5);
}

TEST_F(CircularBufferTest, UnderflowTest)
{
    EXPECT_EQ(buffer->size(), 0);
    EXPECT_TRUE(buffer->empty());

    int popped_value = -1;
    bool running = true;
    thread consumer([&]() {
        popped_value = buffer->pop();
        running = false;
    });

    this_thread::sleep_for(chrono::milliseconds(100));
    EXPECT_TRUE(running);
    EXPECT_EQ(popped_value, -1);

    buffer->push(42);

    consumer.join();
    EXPECT_FALSE(running);
    EXPECT_EQ(popped_value, 42);
    EXPECT_EQ(buffer->size(), 0);
}

TEST_F(CircularBufferTest, MultithreadedTest)
{
    const int num_items = 1000;
    bool running = true;

    thread producer([&]() {
        for (int i = 0; i < num_items; ++i) {
            buffer->push(i);
        }
    });

    vector<int> consumed_items;
    thread consumer([&]() {
        for (int i = 0; i < num_items; ++i) {
            consumed_items.push_back(buffer->pop());
        }
        running = false;
    });

    producer.join();
    consumer.join();

    EXPECT_EQ(consumed_items.size(), num_items);
    for (int i = 0; i < num_items; ++i) {
        EXPECT_EQ(consumed_items[i], i);
    }
    EXPECT_EQ(buffer->size(), 0);
}

TEST_F(CircularBufferTest, WaitTimeTest)
{
    buffer->push(1);
    buffer->push(2);
    buffer->push(3);

    double avg_time = buffer->average_push_wait_time();
    EXPECT_GE(avg_time, 0.0);
    EXPECT_LT(avg_time, 1.0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}