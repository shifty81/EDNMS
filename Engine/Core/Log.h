#pragma once
#include <string>
#include <iostream>

namespace ednms {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Log {
public:
    static void Write(LogLevel level, const std::string& message) {
        const char* prefix = "";
        switch (level) {
            case LogLevel::Debug:   prefix = "[DEBUG] ";   break;
            case LogLevel::Info:    prefix = "[INFO]  ";   break;
            case LogLevel::Warning: prefix = "[WARN]  ";   break;
            case LogLevel::Error:   prefix = "[ERROR] ";   break;
        }
        std::cout << prefix << message << std::endl;
    }

    static void Debug(const std::string& msg)   { Write(LogLevel::Debug, msg); }
    static void Info(const std::string& msg)     { Write(LogLevel::Info, msg); }
    static void Warning(const std::string& msg)  { Write(LogLevel::Warning, msg); }
    static void Error(const std::string& msg)    { Write(LogLevel::Error, msg); }
};

} // namespace ednms
