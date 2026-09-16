#include "EventSystem.h"

std::vector<EditorEvent> EventSystem::m_events;

void EventSystem::push(const EditorEvent& event) {
    m_events.push_back(event);
}

const std::vector<EditorEvent>& EventSystem::getEvents() {
    return m_events;
}

void EventSystem::clear() {
    m_events.clear();
}