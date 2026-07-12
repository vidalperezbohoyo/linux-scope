#pragma once

#include <iostream>

#include "Singleton.h"

class Log : public Singleton<Log>
{
public:
    friend class Singleton<Log>;

    enum class Level
    {
        Error,
        Warning,
        Info,
        Debug
    };

    void set_level(Level level)
    {
        level_ = level;
    }

    template<typename... Args>
    void error(Args&&... args)
    {
        print(Level::Error, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(Args&&... args)
    {
        print(Level::Warning, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(Args&&... args)
    {
        print(Level::Info, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(Args&&... args)
    {
        print(Level::Debug, std::forward<Args>(args)...);
    }

private:
    Log() = default;

    template<typename... Args>
    void print(Level level, Args&&... args)
    {
        if (level > level_)
            return;

        std::cout << "[" << level_to_string(level) << "] ";

        (std::cout << ... << std::forward<Args>(args));

        std::cout << '\n';
    }

    static const char* level_to_string(Level level)
    {
        switch (level)
        {
            case Level::Error:   return "ERROR";
            case Level::Warning: return "WARN";
            case Level::Info:    return "INFO";
            case Level::Debug:   return "DEBUG";
        }

        return "";
    }

    Level level_ = Level::Debug;
};

#define LOG_ERROR(...)   Log::instance().error(__VA_ARGS__)
#define LOG_WARNING(...) Log::instance().warning(__VA_ARGS__)
#define LOG_INFO(...)    Log::instance().info(__VA_ARGS__)
#define LOG_DEBUG(...)   Log::instance().debug(__VA_ARGS__)