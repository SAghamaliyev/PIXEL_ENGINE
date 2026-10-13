#pragma once

#include <vector>

#include "Core/EventSystem/EventTypes.h"

class EventSystem {
public:
    static void pushEvent(const EditorEvent& event);
    static std::vector<EditorEvent> getEvents();

private:
    static void clearEvents();
    static std::vector<EditorEvent> m_events;
};
