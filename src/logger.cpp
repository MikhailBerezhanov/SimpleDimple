#include "logger.h"

#include <array>
#include <vector>
#include <mutex>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;

namespace
{
    class DummyLogger : public GameEngine::ILogger
    {
        void Log(GameEngine::LogLevel, const char*) noexcept override {}
        void Log(GameEngine::LogLevel, const std::string&) noexcept override {}
        void Log(GameEngine::LogLevel, const std::string_view) noexcept override {}
        void Log(GameEngine::LogLevel, const std::stringstream&) noexcept override {}
    };

    std::vector<std::unique_ptr<GameEngine::ILogChannel>> g_channels;
    std::shared_ptr<GameEngine::ILogger> g_globalLogger = std::make_shared<DummyLogger>();
    GameEngine::LogLevel g_LogLevel = GameEngine::LogLevel::Debug;
    std::recursive_mutex g_logLock;

    void Log(GameEngine::LogLevel level, const std::string_view message) noexcept
    {
        try
        {
            const auto timestamp = std::chrono::system_clock::now();
            
            std::stringstream threadIdStream;
            threadIdStream << std::setw(8) << std::hex << std::uppercase << std::this_thread::get_id();
            const std::string threadIdString = std::move(threadIdStream).str();

            const std::scoped_lock lock(g_logLock);
            if (g_LogLevel < level)
            {
                return;
            }

            for (auto& channel : g_channels)
            {
                channel->Log(timestamp, threadIdString, level, message);
            }
        }
        catch (...)
        {
        }
    }
} // namespace

namespace GameEngine
{
    static void LogToStream(std::ostream& stream, std::chrono::system_clock::time_point timestamp, const std::string_view threadId, LogLevel level, const std::string_view message)
    {
        std::time_t t = std::chrono::system_clock::to_time_t(timestamp);
        std::tm tm;
#ifdef WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        
        stream
            << std::put_time<char>(&tm, "%m%d-%H:%M:%S") << '.' << std::setw(3) << std::setfill('0')
            << (std::chrono::time_point_cast<std::chrono::milliseconds>(timestamp).time_since_epoch()).count() % 1000
            << ' ' << threadId << ' ' << "EWID"[static_cast<int>(level)] << ' ' << message;

        if (message.back() == '\n' || message.back() == '\r')
        {
            stream << std::flush;
        }
        else
        {
            stream << std::endl;
        }
    }

    class FileLogChannel final : public ILogChannel
    {
    public:
        explicit FileLogChannel(const fs::path& filename, size_t maxLogFileBytes)
            : m_maxLogFileBytes(maxLogFileBytes)
            , m_filePath(filename)
            , m_bakFilePath(fs::path(filename).concat(".bak"))

        {
            if (m_maxLogFileBytes <= 0)
            {
                throw std::runtime_error("FileLogChannel expects m_maxLogFileBytes > 0");
            }
            if (!Initialize())
            {
                throw std::runtime_error("FileLogChannel Initialize() failed");
            }
        }

        void Log(std::chrono::system_clock::time_point timestamp, const std::string_view threadId, LogLevel level, const std::string_view message) noexcept override
        {
            try
            {
                if (!m_file.is_open() && !Initialize())
                {
                    return;
                }

                if (m_file.fail() || m_file.bad())
                {
                    // restore stream when e.g. disk full on last write
                    m_file.clear();
                }

                LogToStream(m_file, timestamp, threadId, level, message);

                if (m_file.tellp() > m_maxLogFileBytes)
                {
                    m_file.close();
                }
            }
            catch (...)
            {
            }
        }

    private:
        bool Initialize()
        {
            std::error_code ec;
            fs::rename(m_filePath, m_bakFilePath, ec);
            m_file.open(m_filePath, std::ios::out | std::ios::trunc);
            return m_file.is_open();
        }

        const size_t m_maxLogFileBytes;
        const fs::path m_filePath;
        const fs::path m_bakFilePath;
        std::ofstream m_file;
    };

    class StdoutLogChannel final : public ILogChannel
    {
    public:
        void Log(std::chrono::system_clock::time_point timestamp, const std::string_view threadId, LogLevel level, const std::string_view message) noexcept override
        {
            try
            {
                LogToStream(std::cout, timestamp, threadId, level, message);
            }
            catch (...)
            {
            }
        }
    };

    class GlobalLogger final : public ILogger
    {
    public:
        void Log(LogLevel level, const char* LogMessage) noexcept override
        {
            Log(level, LogMessage ? std::string_view{ LogMessage } : std::string_view{ "(nullptr)" });
        }
        void Log(LogLevel level, const std::string& LogMessage) noexcept override
        {
            Log(level, std::string_view{ LogMessage });
        }
        void Log(LogLevel level, const std::string_view LogMessage) noexcept override
        {
            ::Log(level, LogMessage);
        }
        void Log(LogLevel level, const std::stringstream& stream) noexcept override
        {
            try
            {
                ::Log(level, stream.str());
            }
            catch (...)
            {
            }
        }
    };

    class PrefixLogger final : public ILogger
    {
    public:
        PrefixLogger(std::string_view prefix, const std::shared_ptr<ILogger>& baseLogger)
            : m_prefix(prefix)
            , m_baseLogger(baseLogger)
        {
            if (!m_baseLogger)
            {
                throw std::runtime_error("PrefixLogger expects m_baseLogger");
            }
        }

        void Log(LogLevel level, const char* LogMessage) noexcept override
        {
            Log(level, LogMessage ? std::string_view{ LogMessage } : std::string_view{ "(nullptr)" });
        }
        void Log(LogLevel level, const std::string& LogMessage) noexcept override
        {
            Log(level, std::string_view{ LogMessage });
        }
        void Log(LogLevel level, const std::string_view LogMessage) noexcept override
        {
            m_baseLogger->Log(level, (m_prefix + '\t').append(LogMessage));
        }
        void Log(LogLevel level, const std::stringstream& stream) noexcept override
        {
            try
            {
                Log(level, stream.str());
            }
            catch (...)
            {
            }
        }
    private:
        const std::string m_prefix;
        const std::shared_ptr<ILogger> m_baseLogger;
    };

    LoggerInitializer::LoggerInitializer(LogLevel level)
    {
        InitLogger(level);
    }

    LoggerInitializer::~LoggerInitializer()
    {
        FinishLogger();
    }

    Logable::Logable(const std::shared_ptr<ILogger>& logger)
        : m_logger(logger)
    {
        if (!m_logger)
        {
            throw std::runtime_error("Logable expects m_logger");
        }
    }

    Logable::Logable(const std::string_view prefix)
        : m_logger(CreatePrefixLogger(prefix, ::GetLogger()))
    {

    }

    Logable::Logable(const std::string_view prefix, const std::shared_ptr<ILogger>& logger)
        : m_logger(CreatePrefixLogger(prefix, logger))
    {
        if (!m_logger)
        {
            throw std::runtime_error("Logable expects m_logger");
        }
    }

    const std::shared_ptr<ILogger>& Logable::GetLogger() const noexcept
    {
        return m_logger;
    }

    void InitLogger(LogLevel level)
    {
        const std::scoped_lock lock(g_logLock);
        g_globalLogger = std::make_shared<GlobalLogger>();
        g_channels.clear();
        g_LogLevel = level;
    }

    void SetLogLevel(LogLevel level)
    {
        const std::scoped_lock lock(g_logLock);
        g_LogLevel = level;
    }

    void FinishLogger()
    {
        const std::scoped_lock lock(g_logLock);

        Log(LogLevel::Info, "Log finished.");
        g_channels.clear();
        g_globalLogger = std::make_shared<DummyLogger>();
    }

    void AddLogHandler(std::unique_ptr<ILogChannel> writer)
    {
        // EXPECT(writer);
        const std::scoped_lock lock(g_logLock);
        g_channels.push_back(std::move(writer));
    }

    std::unique_ptr<ILogChannel> CreateFileLogChannel(const fs::path& fileName, size_t maxLogFileBytes)
    {
        return std::make_unique<FileLogChannel>(fileName, maxLogFileBytes);
    }

    std::unique_ptr<ILogChannel> CreateStdoutLogChannel()
    {
        return std::make_unique<StdoutLogChannel>();
    }

    std::shared_ptr<ILogger> CreatePrefixLogger(const std::string_view prefix, const std::shared_ptr<ILogger>& baseLogger)
    {
        return std::make_shared<PrefixLogger>(prefix, baseLogger);
    }

    std::shared_ptr<ILogger> CreateDummyLogger()
    {
        return std::make_shared<DummyLogger>();
    }
    
}; // namespace GameEngine

const std::shared_ptr<GameEngine::ILogger>& GetLogger() noexcept
{
    return g_globalLogger;
}
