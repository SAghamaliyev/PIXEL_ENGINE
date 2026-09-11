#pragma once

#include "../EditorLayout.h"
#include "../EditorEvents.h"
#include "../../Logger/Logger.h"

#include <string>
#include <vector>

class ConsolePanel {
public:
    using LogEntry = ::LogEntry;

    void setEventQueue(std::vector<EditorEvent>* events);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    void clearLogs();

private:
    std::vector<EditorEvent>* m_events = nullptr;
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
