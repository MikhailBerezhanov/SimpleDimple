#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <sstream>

class Logger
{
private:
    std::ofstream m_info_file;
    std::ofstream m_err_file;
    std::ofstream m_dbg_file;
    size_t m_level_threshold;
    bool m_no_stdout;

    enum class LOG_MODE
    {
        INFO,
        ERROR,
        DEBUG
    };

    std::stringstream get_wrapped_date() const;

    void generic_log(const std::string &message, LOG_MODE mode, size_t level);

public:
    Logger(const std::string &prefix, size_t max_depth = 0, bool nostdout = false);
    // disable copies
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void info(const std::string &message, size_t depth = 0);
    void error(const std::string &message, size_t depth = 0);
    void debug(const std::string &message, size_t depth = 0);
};

#endif