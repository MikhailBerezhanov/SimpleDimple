#include <ctime>
#include <iomanip>
#include <stdexcept>

#include "config.h"
#include "logger.h"
#include "cppzip.h"

// create string representation using redefined macro
#define LOG_X(name) #name,
const char *log_levels[] = {_LOG_LEVELS_};
#undef LOG_X

Logger::LogFileHdl::LogFileHdl(const std::string &name)
    : m_name(name)
    , m_file(name, std::ios::app)
    , m_size(0)
{
    if (!m_file.is_open())
    {
        throw std::runtime_error("Unable to create " + name);
    }
    m_size = get_size();
}

const std::string &Logger::LogFileHdl::get_name() const
{
    return m_name;
}

void Logger::LogFileHdl::write(const std::string &str)
{
    m_file << str;
    m_size += str.size();
    m_file.flush();
}

size_t Logger::LogFileHdl::get_size() const
{
    if (m_size == 0)
    {
        std::ifstream file(m_name, std::ios::binary | std::ios::ate);
        auto size = file.tellg();
    }

    return m_size;
}

void Logger::LogFileHdl::truncate() 
{
    // reopen to clear file
    m_file.close();
    m_file.open(m_name, std::ios::trunc);
    m_size = 0;
}

Logger::Logger(const std::string &name, size_t max_debug_verbosity, bool nostdout)
    : m_log_file(name)
    , m_max_debug_verbosity(max_debug_verbosity)
    , m_no_stdout(nostdout)
    , m_date_format(LOG_DATE_FORMAT_DEFAULT)
{}

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

void Logger::log(const std::string &message, LOG_LEVEL level)
{
    auto ss = get_wrapped_date();
    auto log_level_int = static_cast<size_t>(level);
    ss << " [" << log_levels[log_level_int] << "]: " << message << "\n";

    auto *out_stream = &std::cout;
    bool should_output_stdout = !m_no_stdout;
    bool should_output_log = true;

    switch (level)
    {
        case LOG_LEVEL::INFO:
            break;
        case LOG_LEVEL::DEBUG_1:
        case LOG_LEVEL::DEBUG_2:
        case LOG_LEVEL::DEBUG_3:
            if (log_level_int >= m_max_debug_verbosity)
            {
                should_output_stdout = false;
                should_output_log = false;
            }
            break;
        case LOG_LEVEL::ERROR:
        case LOG_LEVEL::WARN:
        case LOG_LEVEL::FATAL:
            out_stream = &std::cerr;
            break;
    }

    if (should_output_stdout) {
        *out_stream << ss.str();
    }

    if (should_output_log) {
        if (m_log_file.get_size() >= LOG_MAX_SIZE)
        {
            auto zip_ss = get_wrapped_date();
            auto log_full_name = m_log_file.get_name();
            auto zip_name = log_full_name + "_" + zip_ss.str() + ".zip";
            auto log_short_name = log_full_name.substr(log_full_name.find_last_of('/') + 1);
            OZIP zip(zip_name);
            zip.add_existing_file(log_full_name, log_short_name);
            // reopen to clear file
            m_log_file.truncate();
        }

        m_log_file.write(ss.str());
    }
    
}

void Logger::info(const std::string &message)
{
    log(message, LOG_LEVEL::INFO);
}

void Logger::error(const std::string &message)
{
    log(message, LOG_LEVEL::ERROR);
}

void Logger::debug(const std::string &message, size_t depth)
{
    auto tmp = static_cast<size_t>(LOG_LEVEL::DEBUG_1) + depth;
    auto dbg_level = static_cast<LOG_LEVEL>(tmp);
    if (dbg_level > LOG_LEVEL::DEBUG_3) {
        dbg_level = LOG_LEVEL::DEBUG_3;
    }
    log(message, dbg_level);
}