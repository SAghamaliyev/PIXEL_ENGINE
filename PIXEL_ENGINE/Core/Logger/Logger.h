#pragma once

#include <queue>
#include <string>

enum Type { LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG };

struct LogEntry {
    Type type;
    std::string message;
};

class Logger {
public:
    // Add Log's type and then message
    static void addLog(Type LogType, const std::string& message);

    // Clear all Logs from LogBuffer
    static void clearLogs();

    static std::queue<LogEntry> getLogs();

private:
    static std::queue<LogEntry> m_logBuffer;
};
