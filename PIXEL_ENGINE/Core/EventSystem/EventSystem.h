#pragma once
#include "EventTypes.h"

#include <vector>

class EventSystem {
public:
    static void pushEvent(const EditorEvent& event);
    static std::vector<EditorEvent> getEventsList();

private:
    static void clear();
    static std::vector<EditorEvent> m_events;
};
