#include <Logger.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <algorithm>
#include <cstring>

int main(int argc, char* argv[])
{
    using namespace GameEngine;

    const LoggerInitializer logger(LogLevel::TRACE);

    const bool needStdoutLogs = std::any_of(argv, argv + argc, [](const char* arg) { return !std::strcmp("--log-stdout", arg); });

    if (needStdoutLogs)
    {
        AddLogHandler(CreateStdoutLogChannel());
    }

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}