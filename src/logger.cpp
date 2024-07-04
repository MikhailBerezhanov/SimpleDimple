#include <ctime>
#include <iomanip>
#include <stdexcept>

#include "logger.h"

// create string representation using redefined macro
#define LOG_X(name) #name,
const char *log_levels[] = {_LOG_LEVELS_};
#undef LOG_X

Logger::Logger(const std::string &prefix, size_t max_debug_verbosity, bool nostdout)
    : m_info_file(prefix + "-info.log")
    , m_err_file(prefix + "-error.log")
    , m_dbg_file(prefix + "-debug.log")
    , m_max_debug_verbosity(max_debug_verbosity)
    , m_no_stdout(nostdout)
    , m_date_format("%c, %Z")
{
    if (!m_info_file.is_open())
    {
        throw std::runtime_error("Unable to create " + prefix + "-info.log");
    }
    if (!m_err_file.is_open())
    {
        throw std::runtime_error("Unable to create " + prefix + "-error.log");
    }
    if (!m_dbg_file.is_open())
    {
        throw std::runtime_error("Unable to create " + prefix + "-error.log");
    }
}

void Logger::set_date_format(const char *fmt)
{
    m_date_format = fmt;
}

std::stringstream Logger::get_wrapped_date() const
{
    auto t = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), m_date_format.c_str());
    return ss;
}

void Logger::log(const std::string &message, LOG_LEVEL level, size_t debug_verbosity)
{
    auto ss = get_wrapped_date();
    ss << " [" << log_levels[static_cast<size_t>(level)] << "]: " << message << "\n";
    switch (level)
    {
        case LOG_LEVEL::INFO:
        {
            if (!m_no_stdout)
                std::cout << ss.str();
            m_info_file << ss.str();
            break;
        }
        case LOG_LEVEL::DEBUG:
        {
            if (debug_verbosity < m_max_debug_verbosity)
            {
                if (!m_no_stdout)
                    std::cout << ss.str();
                m_dbg_file << ss.str();
            }
            break;
        }
        case LOG_LEVEL::ERROR:
        case LOG_LEVEL::WARN:
        case LOG_LEVEL::FATAL:
        {
            if (!m_no_stdout)
                std::cerr << ss.str();
            m_err_file << ss.str();
            break;
        }
    }
}

void Logger::info(const std::string &message)
{
    log(message, LOG_LEVEL::INFO, 0);
}

void Logger::error(const std::string &message)
{
    log(message, LOG_LEVEL::ERROR, 0);
}

void Logger::debug(const std::string &message, size_t depth)
{
    log(message, LOG_LEVEL::DEBUG, depth);
}