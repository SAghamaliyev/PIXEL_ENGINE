#include "Logger.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::addLog(LogEntry::Type type, const std::string& message) {
    m_logBuffer.push({ type, message });
}

std::vector<Logger::LogEntry> Logger::flushLogs() {
    std::vector<LogEntry> logs;
    while (!m_logBuffer.empty()) {
        logs.push_back(m_logBuffer.front());
        m_logBuffer.pop();
    }
    return logs;
}

void Logger::clearLogs() {
    std::queue<LogEntry> empty;
    std::swap(m_logBuffer, empty);
}
