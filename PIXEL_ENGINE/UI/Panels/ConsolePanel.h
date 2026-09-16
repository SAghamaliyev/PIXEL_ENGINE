#pragma once

#include "../EditorLayout.h"
#include "../../EventSystem/EventSystem.h"
#include "../../Logger/Logger.h"

#include <string>
#include <vector>

class ConsolePanel {
public:
    using LogEntry = ::LogEntry;

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

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

    void updateLogsFromBuffer();
    void pushEvent(const EditorEvent& event);
};
