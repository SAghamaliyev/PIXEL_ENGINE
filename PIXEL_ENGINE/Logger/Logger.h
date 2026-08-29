#pragma once

#include <string>
#include <queue>
#include <vector>

class Logger {
public:
    struct LogEntry {
        enum Type { LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG };
        Type type;
        std::string message;
    };

    static Logger& getInstance();

    void addLog(LogEntry::Type type, const std::string& message);
    std::vector<LogEntry> flushLogs();
    void clearLogs();

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::queue<LogEntry> m_logBuffer;
};
