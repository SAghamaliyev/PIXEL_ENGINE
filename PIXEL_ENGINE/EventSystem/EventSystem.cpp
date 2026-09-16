#include "EventSystem.h"

void EventSystem::push(const EditorEvent& event) {
    m_events.push_back(event);
}

const std::vector<EditorEvent>& EventSystem::getEvents() const{
    return m_events;
}

void EventSystem::clear() {
    m_events.clear();
}