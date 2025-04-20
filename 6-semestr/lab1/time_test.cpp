#include "time.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace my_namespace;
using namespace my_namespace::literals;

TEST(TimeTest, Constructor)
{
    Time t1(10, 30, 45);
    EXPECT_EQ(t1, Time(10, 30, 45));

    EXPECT_THROW(Time(-1, 0, 0), invalid_argument);
    EXPECT_THROW(Time(0, -1, 0), invalid_argument);
    EXPECT_THROW(Time(0, 0, -1), invalid_argument);
}

TEST(TimeTest, AdditionWithTimeSpan)
{
    Time t1(10, 30, 0);
    TimeSpan ts1 = 2_h + 30_m;
    Time t2 = t1 + ts1;
    EXPECT_EQ(t2, Time(13, 0, 0));
}

TEST(TimeTest, SubtractionWithTimeSpan)
{
    Time t1(13, 0, 0);
    TimeSpan ts1 = 2_h + 30_m;
    Time t2 = t1 - ts1;
    EXPECT_EQ(t2, Time(10, 30, 0));
}

TEST(TimeTest, SubtractionOfTimes)
{
    Time t1(13, 0, 0);
    Time t2(10, 30, 0);
    TimeSpan ts = t1 - t2;
    EXPECT_EQ(ts, TimeSpan(0, 2, 30, 0));
}

TEST(TimeTest, Equality)
{
    Time t1(10, 30, 0);
    Time t2(10, 30, 0);
    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 != t2);
}

TEST(TimeTest, Inequality)
{
    Time t1(10, 30, 0);
    Time t2(11, 0, 0);
    EXPECT_TRUE(t1 != t2);
    EXPECT_FALSE(t1 == t2);
}

TEST(TimeTest, LessThan)
{
    Time t1(10, 30, 0);
    Time t2(11, 0, 0);
    EXPECT_TRUE(t1 < t2);
    EXPECT_FALSE(t2 < t1);
}

TEST(TimeTest, LessThanOrEqual)
{
    Time t1(10, 30, 0);
    Time t2(10, 30, 0);
    Time t3(11, 0, 0);
    EXPECT_TRUE(t1 <= t2);
    EXPECT_TRUE(t1 <= t3);
    EXPECT_FALSE(t3 <= t1);
}

TEST(TimeTest, GreaterThan)
{
    Time t1(11, 0, 0);
    Time t2(10, 30, 0);
    EXPECT_TRUE(t1 > t2);
    EXPECT_FALSE(t2 > t1);
}

TEST(TimeTest, GreaterThanOrEqual)
{
    Time t1(11, 0, 0);
    Time t2(10, 30, 0);
    Time t3(11, 0, 0);
    EXPECT_TRUE(t1 >= t2);
    EXPECT_TRUE(t1 >= t3);
    EXPECT_FALSE(t2 >= t1);
}

TEST(TimeTest, OutputStream)
{
    Time t1(10, 30, 45);
    ostringstream oss;
    oss << t1;
    EXPECT_EQ(oss.str(), "10h 30m 45s");
}

TEST(TimeSpanTest, Constructor)
{
    TimeSpan ts1(1, 2, 30, 0);
    EXPECT_EQ(ts1, TimeSpan(1, 2, 30, 0));

    EXPECT_THROW(TimeSpan(-1, 0, 0, 0), invalid_argument);
    EXPECT_THROW(TimeSpan(0, -1, 0, 0), invalid_argument);
    EXPECT_THROW(TimeSpan(0, 0, -1, 0), invalid_argument);
    EXPECT_THROW(TimeSpan(0, 0, 0, -1), invalid_argument);
}

TEST(TimeSpanTest, Addition)
{
    TimeSpan ts1 = 1_d + 2_h + 30_m;
    TimeSpan ts2 = 0_d + 1_h + 30_m;
    TimeSpan ts3 = ts1 + ts2;
    EXPECT_EQ(ts3, TimeSpan(1, 4, 0, 0));
}

TEST(TimeSpanTest, Subtraction)
{
    TimeSpan ts1 = 1_d + 4_h + 0_m;
    TimeSpan ts2 = 0_d + 1_h + 30_m;
    TimeSpan ts3 = ts1 - ts2;
    EXPECT_EQ(ts3, TimeSpan(1, 2, 30, 0));
}

TEST(TimeSpanTest, Equality)
{
    TimeSpan ts1(1, 2, 30, 0);
    TimeSpan ts2(1, 2, 30, 0);
    EXPECT_TRUE(ts1 == ts2);
    EXPECT_FALSE(ts1 != ts2);
}

TEST(TimeSpanTest, Inequality)
{
    TimeSpan ts1(1, 2, 30, 0);
    TimeSpan ts2(1, 3, 0, 0);
    EXPECT_TRUE(ts1 != ts2);
    EXPECT_FALSE(ts1 == ts2);
}

TEST(TimeSpanTest, LessThan)
{
    TimeSpan ts1(1, 2, 30, 0);
    TimeSpan ts2(1, 3, 0, 0);
    EXPECT_TRUE(ts1 < ts2);
    EXPECT_FALSE(ts2 < ts1);
}

TEST(TimeSpanTest, LessThanOrEqual)
{
    TimeSpan ts1(1, 2, 30, 0);
    TimeSpan ts2(1, 2, 30, 0);
    TimeSpan ts3(1, 3, 0, 0);
    EXPECT_TRUE(ts1 <= ts2);
    EXPECT_TRUE(ts1 <= ts3);
    EXPECT_FALSE(ts3 <= ts1);
}

TEST(TimeSpanTest, GreaterThan)
{
    TimeSpan ts1(1, 3, 0, 0);
    TimeSpan ts2(1, 2, 30, 0);
    EXPECT_TRUE(ts1 > ts2);
    EXPECT_FALSE(ts2 > ts1);
}

TEST(TimeSpanTest, GreaterThanOrEqual)
{
    TimeSpan ts1(1, 3, 0, 0);
    TimeSpan ts2(1, 2, 30, 0);
    TimeSpan ts3(1, 3, 0, 0);
    EXPECT_TRUE(ts1 >= ts2);
    EXPECT_TRUE(ts1 >= ts3);
    EXPECT_FALSE(ts2 >= ts1);
}

TEST(TimeSpanTest, OutputStream)
{
    TimeSpan ts1(1, 2, 30, 45);
    ostringstream oss;
    oss << ts1;
    EXPECT_EQ(oss.str(), "1d 2h 30m 45s");
}

TEST(LiteralsTest, Days)
{
    TimeSpan ts = 1_d;
    EXPECT_EQ(ts, TimeSpan(1, 0, 0, 0));
}

TEST(LiteralsTest, Hours)
{
    TimeSpan ts = 2_h;
    EXPECT_EQ(ts, TimeSpan(0, 2, 0, 0));
}

TEST(LiteralsTest, Minutes)
{
    TimeSpan ts = 30_m;
    EXPECT_EQ(ts, TimeSpan(0, 0, 30, 0));
}

TEST(LiteralsTest, Seconds)
{
    TimeSpan ts = 45_s;
    EXPECT_EQ(ts, TimeSpan(0, 0, 0, 45));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}