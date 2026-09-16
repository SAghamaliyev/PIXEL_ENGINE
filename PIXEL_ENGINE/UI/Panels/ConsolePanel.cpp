#include "ConsolePanel.h"
#include "../Materials/imgui.h"
#include "../../Logger/Logger.h"

void ConsolePanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    updateLogsFromBuffer();

    applyPanelRect(layout.console);
    ImGui::Begin("Console", nullptr, kEditorPanelWindowFlags);

    if (ImGui::Button("Clear")) {
        // /FLAG ClearConsole: UI requests clearing console output.
        pushEvent(EditorEvent{ EditorEventType::ClearConsole });
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

    ImGui::Separator();

    const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

    for (const LogEntry& log : m_logs) {
        if (log.type == LOG_INFO && !m_filterInfo) continue;
        if (log.type == LOG_WARNING && !m_filterWarning) continue;
        if (log.type == LOG_ERROR && !m_filterError) continue;
        if (log.type == LOG_DEBUG && !m_filterDebug) continue;

        ImVec4 color;
        const char* prefix = "[LOG]";
        switch (log.type) {
            case LOG_INFO:
                color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                prefix = "[INFO]";
                break;
            case LOG_WARNING:
                color = ImVec4(1.0f, 0.85f, 0.0f, 1.0f);
                prefix = "[WARNING]";
                break;
            case LOG_ERROR:
                color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                prefix = "[ERROR]";
                break;
            case LOG_DEBUG:
                color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                prefix = "[DEBUG]";
                break;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s %s", prefix, log.message.c_str());
        ImGui::PopStyleColor();
    }

    if (m_scrollToBottom && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::Separator();

    bool reclaimFocus = false;
    ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##ConsoleInput", m_commandBuffer, sizeof(m_commandBuffer), inputFlags)) {
        if (m_commandBuffer[0] != '\0') {
            const std::string command = m_commandBuffer;

            // /FLAG SubmitConsoleCommand: UI requests command execution outside the UI layer.
            EditorEvent event;
            event.type = EditorEventType::SubmitConsoleCommand;
            event.message = command;
            pushEvent(event);

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

void ConsolePanel::pushEvent(const EditorEvent& event) {
    EventSystem::push(event);
}
