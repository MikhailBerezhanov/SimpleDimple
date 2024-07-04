
#include "logger.h"
#include <ctime>
#include <iomanip>
#include <stdexcept>

Logger::Logger(const std::string &prefix, size_t max_depth, bool nostdout)
    : m_info_file(prefix + "-info.log")
    , m_err_file(prefix + "-error.log")
    , m_dbg_file(prefix + "-debug.log")
    , m_level_threshold(max_depth)
    , m_no_stdout(nostdout)
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

std::stringstream Logger::get_wrapped_date() const
{
    auto t = std::time(nullptr);
    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&t), "%c, %Z") << "]";
    return ss;
}

void Logger::generic_log(const std::string &message, LOG_MODE mode, size_t level)
{
    if (level <= m_level_threshold)
    {
        auto ss = get_wrapped_date();

        switch (mode)
        {
            case LOG_MODE::INFO:
            {
                ss << " INFO: " << message << "\n";
                if (!m_no_stdout)
                    std::cout << ss.str();
                m_info_file << ss.str();
                break;
            }
            case LOG_MODE::DEBUG:
            {
                ss << " DEBUG: " << message << "\n";
                if (!m_no_stdout)
                    std::cout << ss.str();
                m_dbg_file << ss.str();
                break;
            }
            case LOG_MODE::ERROR:
            {
                ss << " ERROR: " << message << "\n";
                if (!m_no_stdout)
                    std::cerr << ss.str();
                m_err_file << ss.str();
                break;
            }
        }
    }
}

void Logger::info(const std::string &message, size_t depth)
{
    generic_log(message, LOG_MODE::INFO, depth);
}

void Logger::error(const std::string &message, size_t depth)
{
    generic_log(message, LOG_MODE::ERROR, depth);
}

void Logger::debug(const std::string &message, size_t depth)
{
    generic_log(message, LOG_MODE::DEBUG, depth);
}