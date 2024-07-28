#pragma once
#include "Logger.h"

#include <stdexcept>
#include <sstream>
#include <string>

namespace GameEngine
{
    class CheckFailedException : public std::runtime_error
    {
    public:
        CheckFailedException(const char* file, int line, const std::string& message);
        const char* GetFile() const noexcept;
        int GetLine() const noexcept;
    private:
        const char* const m_file;
        const int m_line;
    };

    void HandleFailedExpectation(ILogger& logger, const char* file, int line, const std::string& message);
    void HandleException(ILogger& logger, const std::string& message = {});
} // namespace GameEngine

#define HANDLE_EXCEPTION() HandleException(*GetLogger())
#define HANDLE_EXCEPTION_MSG(message) HandleException(*GetLogger(), message)

#define CHECK_EX(condition, exType, ...) do { if (!(condition)) throw exType(__VA_ARGS__); } while (false);

#define CHECK_MSG(condition, message) CHECK_EX(condition, \
  GameEngine::CheckFailedException, __FILE__, __LINE__, static_cast<std::ostringstream&&>(std::ostringstream() << message).str())

#define CHECK(condition) CHECK_EX(condition, \
  GameEngine::CheckFailedException, __FILE__, __LINE__, static_cast<std::ostringstream&&>(std::ostringstream() << "Check "#condition" failed").str())

#define EXPECT(condition) \
	do { if (!(condition)) GameEngine::HandleFailedExpectation(*GetLogger(), __FILE__, __LINE__, static_cast<std::ostringstream&&>(std::ostringstream() << #condition).str()); } while (false); 

#define EXPECT_MSG(condition, message) \
	do { if (!(condition)) GameEngine::HandleFailedExpectation(*GetLogger(), __FILE__, __LINE__, static_cast<std::ostringstream&&>(std::ostringstream() << message).str()); } while (false); 
