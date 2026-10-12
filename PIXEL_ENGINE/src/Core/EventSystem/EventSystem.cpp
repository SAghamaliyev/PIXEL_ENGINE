#include "EventSystem.h"

#include <vector>

using namespace std;

vector<EditorEvent> EventSystem::m_events;

void EventSystem::pushEvent(const EditorEvent& event) {
    m_events.push_back(event);
}

vector<EditorEvent> EventSystem::getEvents() {
    vector<EditorEvent> events = m_events;
    clearEvents();
    return events;
}

void EventSystem::clearEvents() {
    m_events.clear();
}
