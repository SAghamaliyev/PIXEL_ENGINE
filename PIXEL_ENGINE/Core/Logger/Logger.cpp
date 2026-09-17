#include "Logger.h"

std::queue<LogEntry> Logger::m_logBuffer;

void Logger::addLog(Type LogType, const std::string& message) {

    LogEntry OurLog = { LogType, message };
    m_logBuffer.push(OurLog);
}

void Logger::clearLogs() {
    std::queue<LogEntry> empty;
    std::swap(m_logBuffer, empty);
}

std::queue<LogEntry> Logger::getLogs() {
    std::queue<LogEntry> temp = m_logBuffer;
    clearLogs();
    return temp;
}
