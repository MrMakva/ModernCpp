#include "list.h"
#include <gtest/gtest.h>

TEST(LinkedListTest, DefaultConstructor)
{
    LinkedList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(LinkedListTest, InitializerListConstructor)
{
    LinkedList<int> list = {1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, CopyConstructor)
{
    LinkedList<int> original = {1, 2, 3};
    LinkedList<int> copy(original);

    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 3);
}

TEST(LinkedListTest, MoveConstructor)
{
    LinkedList<int> original = {1, 2, 3};
    LinkedList<int> moved(move(original));

    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(original.empty());
}

TEST(LinkedListTest, PushFront)
{
    LinkedList<int> list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, PushBack)
{
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(LinkedListTest, PopFront)
{
    LinkedList<int> list = {1, 2, 3};
    list.pop_front();

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 2);
}

TEST(LinkedListTest, PopBack)
{
    LinkedList<int> list = {1, 2, 3};
    list.pop_back();

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back(), 2);
}

TEST(LinkedListTest, Clear)
{
    LinkedList<int> list = {1, 2, 3};
    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(LinkedListTest, Iterator)
{
    LinkedList<int> list = {1, 2, 3};
    auto it = list.begin();

    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, list.end());
}

TEST(LinkedListTest, ReverseIterator)
{
    LinkedList<int> list = {1, 2, 3};
    auto it = list.rbegin();

    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(it, list.rend());
}

TEST(LinkedListTest, ConstIterator)
{
    const LinkedList<int> list = {1, 2, 3};
    auto it = list.cbegin();

    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
}

TEST(LinkedListTest, FindAlgorithm)
{
    LinkedList<int> list = {1, 2, 3, 4, 5};
    auto it = find(list.begin(), list.end(), 3);

    EXPECT_NE(it, list.end());
    EXPECT_EQ(*it, 3);
}

TEST(LinkedListTest, ForEachAlgorithm)
{
    LinkedList<int> list = {1, 2, 3};
    int sum = 0;
    for_each(list.begin(), list.end(), [&sum](int x) { sum += x; });

    EXPECT_EQ(sum, 6);
}

TEST(LinkedListTest, Swap)
{
    LinkedList<int> list1 = {1, 2, 3};
    LinkedList<int> list2 = {4, 5};

    list1.swap(list2);

    EXPECT_EQ(list1.size(), 2);
    EXPECT_EQ(list2.size(), 3);
}

TEST(LinkedListTest, FrontBackExceptions)
{
    LinkedList<int> list;

    EXPECT_THROW(list.front(), out_of_range);
    EXPECT_THROW(list.back(), out_of_range);
}

TEST(LinkedListTest, IteratorExceptions)
{
    LinkedList<int> list = {1};
    auto it = list.begin();

    EXPECT_NO_THROW(++it);
    EXPECT_THROW(++it, out_of_range);
    EXPECT_THROW(*it, out_of_range);
}

TEST(LinkedListTest, SortAlgorithm)
{
    LinkedList<int> list = {5, 3, 1, 4, 2};

    if (list.size() > 1) {
        bool swapped;
        do {
            swapped = false;
            auto it = list.begin();
            auto next = it;
            ++next;

            while (next != list.end()) {
                if (*it > *next) {
                    swap(*it, *next);
                    swapped = true;
                }
                ++it;
                ++next;
            }
        } while (swapped);
    }

    auto it = list.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 3);
    EXPECT_EQ(*it++, 4);
    EXPECT_EQ(*it, 5);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}