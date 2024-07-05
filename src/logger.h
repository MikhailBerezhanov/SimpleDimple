#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <sstream>

// a trick to automatically create a enum and its string representation
#define _LOG_LEVELS_                                                                                                                       \
    LOG_X(INFO)                                                                                                                            \
    LOG_X(WARN)                                                                                                                            \
    LOG_X(ERROR)                                                                                                                           \
    LOG_X(FATAL)                                                                                                                           \
    LOG_X(DEBUG_1)                                                                                                                         \
    LOG_X(DEBUG_2)                                                                                                                         \
    LOG_X(DEBUG_3)
#define LOG_X(name) name,
enum class LOG_LEVEL : unsigned int
{
    _LOG_LEVELS_
};
#undef LOG_X

class Logger
{
private:
    struct log_file
    {
        std::string name;
        std::ofstream file;
        size_t size;
        log_file(const std::string &name, const std::ios_base::openmode mode = std::ios::out);
        // std::ofstream *operator->() { return &file; }
        void write(const std::string &str);
        size_t get_size() const;
    };

    log_file m_info_file;
    log_file m_err_file;
    log_file m_dbg_file;

    size_t m_max_debug_verbosity;
    bool m_no_stdout;
    std::string m_date_format;

    std::stringstream get_wrapped_date() const;

public:
    Logger(const std::string &prefix, size_t max_debug_verbosity = 0, bool nostdout = false);
    // disable copies
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void set_date_format(const char *fmt);

    void log(const std::string &message, LOG_LEVEL level);

    void info(const std::string &message);
    void error(const std::string &message);
    void debug(const std::string &message, size_t depth = 0);
};

#endif