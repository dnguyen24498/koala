#include <iostream>
#include <stdarg.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "LogUtils.h"

#if defined(USE_DLT)
#include <dlt/dlt.h>
DLT_DECLARE_CONTEXT(mainContext);
#endif

#define LOG_APPID "WIFI"

LogUtils* LogUtils::getInstance()
{
    static LogUtils *pLogUtils = nullptr;

    if(pLogUtils == nullptr)
    {
        pLogUtils = new LogUtils();
    }

    return pLogUtils;
}

void LogUtils::registerService(ILogger::Ptr logger) noexcept
{
    mLoggers.push_back(logger);

#if defined(USE_DLT)
    DLT_REGISTER_APP(LOG_APPID, "Koala Wifi Manager");
    DLT_REGISTER_CONTEXT(mainContext, LOG_APPID, "Koala Wifi Manager context");
#endif
}

void LogUtils::out(LogLevel lv, const char* format, ...) noexcept
{
    char buf[BUF_SIZE];
    va_list args;

    va_start(args, format);
    vsnprintf(buf, BUF_SIZE, format, args);
    va_end(args);

    mLoggers[0]->out(lv, buf); //ADB Logcat

}

void LogUtils::out(LogLevel lv, const char* fileName, const unsigned long line, const char* func, const char* text, const uint8_t buf[], unsigned int size) noexcept
{
    std::stringstream ss("");

    ss << "[" << fileName << ":" << line << "][" << func << "]" << text;

    for (unsigned int i=0; i<size; i++)
        ss << " " << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned>(buf[i]);

    mLoggers[0]->out(lv, ss.str().c_str()); //ADB Logcat

}

void Logger::out(LogLevel lv, const char* str) noexcept
{
    if (lv == LogLevel::Verbose)
#if defined(USE_DLT)
        DLT_LOG(mainContext, DLT_LOG_VERBOSE, DLT_STRING(str));
#else
        std::cout << "[V]" << str << std::endl;
#endif
    else if (lv == LogLevel::Debug)
#if defined(USE_DLT)
        DLT_LOG(mainContext, DLT_LOG_DEBUG, DLT_STRING(str));
#else
        std::cout << "[D]" << str << std::endl;
#endif
    else if (lv == LogLevel::Info)
#if defined(USE_DLT)
        DLT_LOG(mainContext, DLT_LOG_INFO, DLT_STRING(str));
#else
        std::cout << "[I]" << str << std::endl;
#endif
    else if (lv == LogLevel::Warn)
#if defined(USE_DLT)
        DLT_LOG(mainContext, DLT_LOG_WARN, DLT_STRING(str));
#else
        std::cout << "[W]" << str << std::endl;
#endif
    else if (lv == LogLevel::Error)
#if defined(USE_DLT)
        DLT_LOG(mainContext, DLT_LOG_ERROR, DLT_STRING(str));
#else
        std::cout << "[E]" << str << std::endl;
#endif
    else if (lv == LogLevel::Fatal)
#if defined(USE_DLT)
        DLT_LOG(mainContext, DLT_LOG_FATAL, DLT_STRING(str));
#else
        std::cout << "[F]" << str << std::endl;
#endif
}

