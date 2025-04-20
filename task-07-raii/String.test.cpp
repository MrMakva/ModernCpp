#include "String.hpp"
#include <gtest/gtest.h>

TEST(StringTest, DefaultConstructor)
{
    const String str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_STREQ(str.c_str(), "");
}

TEST(StringTest, ConstructorWithCString)
{
    const String str("Hello");
    EXPECT_EQ(str.size(), 5);
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST(StringTest, CopyConstructor)
{
    const String str1("Hello");
    const String str2 = str1;
    EXPECT_EQ(str2.size(), 5);
    EXPECT_STREQ(str2.c_str(), "Hello");
}

TEST(StringTest, AssignmentOperator)
{
    const String str1("Hello");
    String str2;
    str2 = str1;
    EXPECT_EQ(str2.size(), 5);
    EXPECT_STREQ(str2.c_str(), "Hello");
}

TEST(StringTest, AtMethod)
{
    String str("Hello");
    EXPECT_EQ(str.at(0), 'H');
    EXPECT_THROW(str.at(10), out_of_range);
}

TEST(StringTest, AtMethodConst)
{
    const String str("Hello");
    EXPECT_EQ(str.at(1), 'e');
    EXPECT_THROW(str.at(10), out_of_range);
}
