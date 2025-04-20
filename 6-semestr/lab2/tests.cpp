#include"ptr.h"
#include <gtest/gtest.h>

TEST(UniquePtrTest, BasicOperations)
{
    UniquePtr<int> ptr(new int(5));
    EXPECT_EQ(*ptr, 5);

    ptr.reset(new int(10));
    EXPECT_EQ(*ptr, 10);
}

TEST(SharedPtrTest, BasicOperations)
{
    SharedPtr<int> ptr1(new int(5));
    SharedPtr<int> ptr2 = ptr1;
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(WeakPtrTest, LockFunction)
{
    SharedPtr<int> shared(new int(42));
    WeakPtr<int> weak(shared);
    auto locked = weak.lock();
    EXPECT_EQ(*locked, 42);
}
