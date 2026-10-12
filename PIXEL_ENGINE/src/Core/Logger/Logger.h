#pragma once

#include <queue>
#include <string>

enum class LogType { LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG };

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
public:
    // Add Log's type and then message
    static void addLog(LogType logType, const std::string& message);

    // Clear all Logs from LogBuffer
    static void clearLogs();

    static std::queue<LogEntry> getLogs();

private:
    static std::queue<LogEntry> m_logBuffer;
};
