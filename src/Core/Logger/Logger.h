#pragma once

#include <queue>
#include <string>

enum class LogType { LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG };

// This struct is used to write information into m_logBuffer
struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
public:
    static void addLog(LogType logType, std::string message);
    static void clearLogs();  // Clear all Logs from m_logBuffer
    static std::queue<LogEntry> getLogs();

private:

    // Queue of logs that are read and shown in terminal each frame
    static std::queue<LogEntry> m_logBuffer;
};
