#include "ConsolePanel.h"

#include "../Theme/EditorTheme.h"
#include "../Materials/imgui.h"
#include "../../Core/EventSystem/EventSystem.h"
#include "../../Core/Logger/Logger.h"

void ConsolePanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    updateLogsFromBuffer();

    applyPanelRect(layout.console);
    ImGui::Begin("##Console", nullptr, kEditorPanelWindowFlags);
    EditorTheme::drawPanelHeader("Console", "Output");

    if (ImGui::Button("Clear")) {
        EditorEvent event;
        event.type = EditorEventType::ClearConsole;
        EventSystem::pushEvent(event);
        clearLogs();
    }

    ImGui::SameLine();
    ImGui::Checkbox("Info", &m_filterInfo);
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &m_filterWarning);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &m_filterError);
    ImGui::SameLine();
    ImGui::Checkbox("Debug", &m_filterDebug);

    const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footerHeight), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (const LogEntry& log : m_logs) {
        if (log.type == LOG_INFO && !m_filterInfo) continue;
        if (log.type == LOG_WARNING && !m_filterWarning) continue;
        if (log.type == LOG_ERROR && !m_filterError) continue;
        if (log.type == LOG_DEBUG && !m_filterDebug) continue;

        ImVec4 color;
        const char* prefix = "[LOG]";
        switch (log.type) {
            case LOG_INFO:
                color = ImVec4(0.78f, 0.80f, 0.84f, 1.0f);
                prefix = "[INFO]";
                break;
            case LOG_WARNING:
                color = ImVec4(1.0f, 0.82f, 0.28f, 1.0f);
                prefix = "[WARNING]";
                break;
            case LOG_ERROR:
                color = ImVec4(1.0f, 0.38f, 0.38f, 1.0f);
                prefix = "[ERROR]";
                break;
            case LOG_DEBUG:
                color = ImVec4(0.55f, 0.58f, 0.64f, 1.0f);
                prefix = "[DEBUG]";
                break;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s  %s", prefix, log.message.c_str());
        ImGui::PopStyleColor();
    }

    if (m_scrollToBottom && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    bool reclaimFocus = false;
    ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputTextWithHint("##ConsoleInput", "Enter command...", m_commandBuffer, sizeof(m_commandBuffer), inputFlags)) {
        if (m_commandBuffer[0] != '\0') {
            const std::string command = m_commandBuffer;

            EditorEvent event;
            event.type = EditorEventType::SubmitConsoleCommand;
            event.info.message = command;
            EventSystem::pushEvent(event);

            Logger::addLog(LOG_INFO, std::string("> ") + command);
            m_commandBuffer[0] = '\0';
            reclaimFocus = true;
            m_scrollToBottom = true;
        }
    }

    if (reclaimFocus) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}

void ConsolePanel::updateLogsFromBuffer() {
    std::queue<LogEntry> newLogsQueue = Logger::getLogs();
    while (!newLogsQueue.empty()) {
        m_logs.push_back(newLogsQueue.front());
        newLogsQueue.pop();
    }
    if (!m_logs.empty()) {
        m_scrollToBottom = true;
    }
}

void ConsolePanel::clearLogs() {
    m_logs.clear();
    Logger::clearLogs();
}
