#ifndef TST_DATABASE_H
#define TST_DATABASE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "database.h"

using namespace testing;

TEST(InputOutput, DatabaseCanBuild)
{
    // This should connect DB if exists, or create a new one if it doesn't
    Database *testDb = Database::getInstance();
    // Test that we can connect to db
    ASSERT_THAT(testDb->connected,true);
}

#endif // TST_DATABASE_H
