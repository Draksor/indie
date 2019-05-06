#pragma once

#include <iostream>

#include "./details/OS.hpp"
#include "./details/StringFormat.hpp"

namespace indie::log 
{
    enum LoggerLevel
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    static inline constexpr char *LevelToStr(LoggerLevel level)
    {
        switch (level) {
            case Trace: return "Trace";
            case Debug: return "Debug";
            case Info: return "Info";
            case Warn: return "Warn";
            case Error: return "Error";
            case Critical: return "Critical";
            default: return "";
        }
    }

    class Logger 
    {
    public:
        inline Logger(const char *name) noexcept :
            _name(name)
        {}
        inline Logger(const std::string &name) noexcept :
            _name(name)
        {}
        inline Logger(const char *name, LoggerLevel level) noexcept :
            _name(name), _level(level)
        {}
        inline Logger(const std::string &name, LoggerLevel level) noexcept :
            _name(name), _level(level)
        {}

        Logger(const Logger &logger) = delete;
        Logger &operator=(const Logger &logger) = delete;

        template <typename ...Args>
        inline void Log(LoggerLevel level, const char *fmt, Args &&...args) const noexcept
        {
            Log(level, FormatMsg(fmt, std::forward<Args>(args)...));
        }
        inline void Log(LoggerLevel level, const std::string &msg) const noexcept
        {
            Log(level, msg.c_str());
        }
        inline void Log(LoggerLevel level, const char *msg) const noexcept
        {
            if (!IsLoggable(level)) {
                return;
            }
            std::cout << "[" << details::OS::GetCurrentTime() << "] [" << LevelToStr(level) << "] [" << _name << "] : " << msg << std::endl;
        }

        template <typename ...Args>
        inline void Trace(const char *fmt, Args &&...args) const noexcept
        {
            Log(LoggerLevel::Trace, fmt, args...);
        }
        inline void Trace(const char *msg) const noexcept
        {
            Log(LoggerLevel::Trace, msg);
        }

        template <typename ...Args>
        inline void Debug(const char *fmt, Args &&...args) const noexcept
        {
            Log(LoggerLevel::Debug, fmt, args...);
        }
        inline void Debug(const char *msg) const noexcept
        {
            Log(LoggerLevel::Debug, msg);
        }

        template <typename ...Args>
        inline void Info(const char *fmt, Args &&...args) const noexcept
        {
            Log(LoggerLevel::Info, fmt, args...);
        }
        inline void Info(const char *msg) const noexcept
        {
            Log(LoggerLevel::Info, msg);
        }

        template <typename ...Args>
        inline void Warn(const char *fmt, Args &&...args) const noexcept
        {
            Log(LoggerLevel::Warn, fmt, args...);
        }
        inline void Warn(const char *msg) const noexcept
        {
            Log(LoggerLevel::Warn, msg);
        }

        template <typename ...Args>
        inline void Error(const char *fmt, Args &&...args) const noexcept
        {
            Log(LoggerLevel::Error, fmt, args...);
        }
        inline void Error(const char *msg) const noexcept
        {
            Log(LoggerLevel::Error, msg);
        }

        template <typename ...Args>
        inline void Critical(const char *fmt, Args &&...args) const noexcept
        {
            Log(LoggerLevel::Critical, fmt, args...);
        }
        inline void Critical(const char *msg) const noexcept
        {
            Log(LoggerLevel::Critical, msg);
        }

        inline bool IsLoggable(LoggerLevel level) const noexcept
        {
            return level >= _level;
        }

        inline static LoggerLevel DefaultLevel() noexcept
        {
#ifdef DEBUG
            return LoggerLevel::Trace;
#else
            return LoggerLevel::Info;
#endif
        }

    private:
        inline void FormatMsg(std::string &to_format, std::size_t &arg_pos) const noexcept
        { }
        template <typename Arg, typename ...Args>
        inline void FormatMsg(std::string &to_format, std::size_t &arg_pos, Arg &&arg, Args &&...args) const noexcept
        {
            std::size_t format_pos = to_format.find("{" + std::to_string(arg_pos) + "}");
            if (format_pos == std::string::npos) {
                return;
            }
            to_format.replace(format_pos, 3, details::StringFormat::ToString(arg));
            arg_pos++;
            FormatMsg(to_format, arg_pos, args...);
        }
        template <typename ...Args>
        inline std::string FormatMsg(const char *fmt, Args &&...args) const noexcept
        {
            std::size_t arg_pos{0};
            std::string to_format{fmt};
            FormatMsg(to_format, arg_pos, args...);
            return to_format;
        }

    private:
        std::string _name;
        LoggerLevel _level{DefaultLevel()};
    };

}