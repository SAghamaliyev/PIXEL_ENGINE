#include "Logger.h"

using namespace std;

queue<LogEntry> Logger::m_logBuffer;

void Logger::addLog(const LogType logType, std::string message) {
    m_logBuffer.push(LogEntry{ logType, std::move(message) });
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
