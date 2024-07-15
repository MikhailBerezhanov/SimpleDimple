#include <ErrorHandling.h>

#include <gtest/gtest.h>

using namespace GameEngine;

TEST(ErrorHandling, ShouldThrowAtFailedExpectation)
{
    ASSERT_THROW(EXPECT(1 == 2), CheckFailedException);
}

TEST(ErrorHandling, ShouldNotThrowAtSucceedExpectation)
{
    ASSERT_NO_THROW(EXPECT(4 == 4));
}

TEST(ErrorHandling, ShouldThrowAtFailedExpectationWithCustomMessage)
{
    ASSERT_THROW(EXPECT_MSG(1 == 2, "Test " << "message"), CheckFailedException);
}

TEST(ErrorHandling, ShouldNotThrowAtSucceedExpectationWithCustomMessage)
{
    ASSERT_NO_THROW(EXPECT_MSG(4 == 4, "Should not be printed"));
}

TEST(ErrorHandling, ShouldCatchFailedCheck)
{
    try
    {
        CHECK(1 > 2);
    }
    catch(const std::exception& e)
    {
        HandleException(*GetLogger());
    }
}

TEST(ErrorHandling, ShouldNotThrowAtSuceedCheck)
{
    ASSERT_NO_THROW(CHECK(3 > 2));
}

TEST(ErrorHandling, ShouldCatchFailedCheckWithCustomMessage)
{
    try
    {
        CHECK_MSG(1 > 2, "My message for failed check");
    }
    catch(const std::exception& e)
    {
        HandleException(*GetLogger());
    }
}