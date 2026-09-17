#include "EventSystem.h"

std::vector<EditorEvent> EventSystem::m_events;

void EventSystem::pushEvent(const EditorEvent& event) {
    m_events.push_back(event);
}

std::vector<EditorEvent> EventSystem::getEventsList() {
    std::vector<EditorEvent> events = m_events;
    clear();
    return events;
}

void EventSystem::clear() {
    m_events.clear();
}
