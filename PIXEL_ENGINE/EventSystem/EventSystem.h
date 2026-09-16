#pragma once
#pragma once
#include "EventTypes.h"

#include <vector>

// ============================================================================
// EventSystem — Central part of adding event flags
// Each modul (UI, Input, Scene, Asset, Render) can push there event,
// and Engine can process them.
// ============================================================================
class EventSystem {
public:

    void push(const EditorEvent& event);

    const std::vector<EditorEvent>& getEvents() const;

    void clear();

private:
    std::vector<EditorEvent> m_events;
};
