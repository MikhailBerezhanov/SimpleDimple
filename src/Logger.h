#pragma once

#include <chrono>
#include <string_view>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <sstream>
#include <filesystem>


#define _LOG_LEVELS_    \
    LOG_X(ERROR)        \
    LOG_X(WARNING)      \
    LOG_X(INFO)         \
    LOG_X(DEBUG)        \
    LOG_X(VERBOSE)      \
    LOG_X(TRACE)        \


namespace GameEngine
{
    namespace fs = std::filesystem;

#define LOG_X(name) name,
    enum class LogLevel : unsigned int
    {
        _LOG_LEVELS_
    };
#undef LOG_X

    struct ILogger
    {
        virtual ~ILogger() = default;

        virtual void Log(LogLevel level, const char* message) noexcept = 0;
        virtual void Log(LogLevel level, const std::string& message) noexcept = 0;
        virtual void Log(LogLevel level, const std::string_view message) noexcept = 0;
        virtual void Log(LogLevel level, const std::stringstream& stream) noexcept = 0;
    };

    struct ILogChannel
    {
        virtual ~ILogChannel() = default;
        virtual void Log(std::chrono::system_clock::time_point timestamp, LogLevel level, const std::string_view message) noexcept = 0;
    };

    void InitLogger(LogLevel level);
    void SetLogLevel(LogLevel level);
    void AddLogHandler(std::unique_ptr<ILogChannel> writer);
    std::unique_ptr<ILogChannel> CreateFileLogChannel(const fs::path& fileName, size_t maxLogFileBytes);
    std::unique_ptr<ILogChannel> CreateStdoutLogChannel();
    void FinishLogger();

    std::shared_ptr<ILogger> CreatePrefixLogger(const std::string_view prefix, const std::shared_ptr<ILogger>& baseLogger);
    std::shared_ptr<ILogger> CreateDummyLogger();

    class LoggerInitializer
    {
    public:
        explicit LoggerInitializer(LogLevel level);
        ~LoggerInitializer();
    };

    class Logable
    {
    protected:
        explicit Logable(const std::shared_ptr<ILogger>& logger);
        explicit Logable(const std::string_view prefix);
        Logable(const std::string_view prefix, const std::shared_ptr<ILogger>& logger);

        Logable(const Logable&) = default;
        Logable(Logable&&) = default;
        Logable& operator=(const Logable&) = default;
        Logable& operator=(Logable&&) = default;
        ~Logable() = default;

        const std::shared_ptr<ILogger>& GetLogger() const noexcept;

    private:
        std::shared_ptr<ILogger> m_logger;
    };

    class LogStreamHelper final : public std::stringstream
    {
    public:
        LogStreamHelper(LogLevel level, ILogger& Logger)
            : m_level(level)
            , m_Logger(Logger)
        {
        }

        LogStreamHelper(const LogStreamHelper&) = delete;
        LogStreamHelper(LogStreamHelper&&) = delete;
        LogStreamHelper& operator=(const LogStreamHelper&) = delete;
        LogStreamHelper& operator=(LogStreamHelper&&) = delete;

        ~LogStreamHelper()
        {
            m_Logger.Log(m_level, *this);
        }

    private:
        const LogLevel m_level;
        ILogger& m_Logger;
    };
} // namespace

const std::shared_ptr<GameEngine::ILogger>& GetLogger() noexcept;

#define LOG_TRACE(message) do { GameEngine::LogStreamHelper t(GameEngine::LogLevel::TRACE, *GetLogger()); t << '[' << __func__ << "():" << __LINE__ << "] " << message; } while (false);
#define LOG_VERBOSE(message) do { GameEngine::LogStreamHelper t(GameEngine::LogLevel::VERBOSE, *GetLogger()); t << message; } while (false);
#define LOG_DEBUG(message) do { GameEngine::LogStreamHelper t(GameEngine::LogLevel::DEBUG, *GetLogger()); t << message; } while (false);
#define LOG_INFO(message) do { GameEngine::LogStreamHelper t(GameEngine::LogLevel::INFO, *GetLogger()); t << message; } while (false);
#define LOG_WARNING(message) do { GameEngine::LogStreamHelper t(GameEngine::LogLevel::WARNING, *GetLogger()); t << message; } while (false);
#define LOG_ERROR(message) do { GameEngine::LogStreamHelper t(GameEngine::LogLevel::ERROR, *GetLogger()); t << message; } while (false);
