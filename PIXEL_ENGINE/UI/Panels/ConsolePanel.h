#pragma once

#include "../EditorLayout.h"

#include <string>
#include <vector>

class ConsolePanel {
public:
    struct LogEntry {
        enum Type { LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG };
        Type type;
        std::string message;
    };

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    void addLog(LogEntry::Type type, const std::string& message);
    void clearLogs();

private:
    bool m_visible = true;
    std::vector<LogEntry> m_logs;
    char m_commandBuffer[256] = "";
    bool m_filterInfo = true;
    bool m_filterWarning = true;
    bool m_filterError = true;
    bool m_filterDebug = true;
    bool m_scrollToBottom = true;
};
