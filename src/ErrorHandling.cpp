#include "ErrorHandling.h"

namespace GameEngine
{
    CheckFailedException::CheckFailedException(const char* file, int line, const std::string& message)
        : std::runtime_error(message)
        , m_file(file)
        , m_line(line)
    {
    }

    const char* CheckFailedException::GetFile() const noexcept
    {
        return m_file;
    }

    int CheckFailedException::GetLine() const noexcept
    {
        return m_line;
    }

    void HandleFailedExpectation(ILogger& logger, const char* file, int line, const std::string& message)
    {
        std::stringstream ss;
        ss << "EXPECTATION FAILED: [" << message << "] at " << file << ":" << line;
        logger.Log(LogLevel::ERROR, ss);

        throw CheckFailedException(file, line, message);
    }

    void HandleException(ILogger& logger, const std::string& message)
    {
        std::stringstream ss;
        if (!message.empty())
        {
            ss << message << ". ";
        }

        try
        {
            throw;
        }
        catch (const CheckFailedException& e)
        {
            ss << "Exception [" << e.what() << "] at " << e.GetFile() << ':' << e.GetLine();
            logger.Log(LogLevel::ERROR, ss);
        }
        catch (const std::exception& e)
        {
            ss << "Exception [" << e.what() << "]";
            logger.Log(LogLevel::ERROR, ss);
        }
    }
} // namespace GameEngine
