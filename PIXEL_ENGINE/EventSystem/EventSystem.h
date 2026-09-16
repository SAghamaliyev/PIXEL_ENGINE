#pragma once
#pragma once
#include "EventTypes.h"

#include <vector>

class EventSystem {
public:
    static void push(const EditorEvent& event);

    static const std::vector<EditorEvent>& getEvents();

    static void clear();

private:
    static std::vector<EditorEvent> m_events;
};
