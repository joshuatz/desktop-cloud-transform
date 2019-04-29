#ifndef TST_DATABASE_H
#define TST_DATABASE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(Database, InputOutput)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

#endif // TST_DATABASE_H
