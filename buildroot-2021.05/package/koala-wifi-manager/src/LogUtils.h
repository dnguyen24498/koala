#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

enum class LogLevel : uint8_t
{
    Debug = 0,
    Verbose,
    Info,
    Warn,
    Error,
    Fatal
};

class ILogger
{
public:
    using Ptr = std::shared_ptr<ILogger>;
    virtual ~ILogger() = default;
    virtual void out(LogLevel lv, const char* str) = 0;
};

class LogUtils
{
public:
    static LogUtils* getInstance();
    void registerService(ILogger::Ptr logger) noexcept;
    void out(LogLevel lv, const char* format, ...) noexcept;
    void out(LogLevel lv, const char* fileName, const unsigned long line, const char* func, const char* text, const uint8_t buf[], unsigned int size) noexcept;

private:
    LogUtils() = default;
    ~LogUtils() = default;
    std::vector<ILogger::Ptr> mLoggers;
    static constexpr uint64_t BUF_SIZE = 1024;
};


class Logger : public ILogger
{
public:
    Logger() = default;
    virtual ~Logger() = default;
    virtual void out(LogLevel lv, const char* str) noexcept;
};

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_FATAL(format, args...) \
    LogUtils::getInstance()->out(LogLevel::Fatal, "[%s:%d][%s] " format, __FILENAME__, __LINE__, __func__, ##args)

#define LOG_ERROR(format, args...) \
    LogUtils::getInstance()->out(LogLevel::Error, "[%s:%d][%s] " format, __FILENAME__, __LINE__, __func__, ##args)

#define LOG_WARN(format, args...) \
    LogUtils::getInstance()->out(LogLevel::Warn, "[%s:%d][%s] " format, __FILENAME__, __LINE__, __func__, ##args)

#define LOG_INFO(format, args...) \
    LogUtils::getInstance()->out(LogLevel::Info, "[%s:%d][%s] " format, __FILENAME__, __LINE__, __func__, ##args)

#define LOG_DEBUG(format, args...) \
    LogUtils::getInstance()->out(LogLevel::Debug, "[%s:%d][%s] " format, __FILENAME__, __LINE__, __func__, ##args)

#define LOG_VERBOSE(format, args...) \
    LogUtils::getInstance()->out(LogLevel::Verbose, "[%s:%d][%s] " format, __FILENAME__, __LINE__, __func__, ##args)

#define LOG_BUF(text, buf, size) \
    LogUtils::getInstance()->out(LogLevel::Info, __FILENAME__, __LINE__, __func__, text, buf, size)

#endif  //LOG_SERVICE_H__
