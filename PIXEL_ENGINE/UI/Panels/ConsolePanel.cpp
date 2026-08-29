#include "ConsolePanel.h"
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
        if (log.type == LogEntry::LOG_INFO && !m_filterInfo) continue;
        if (log.type == LogEntry::LOG_WARNING && !m_filterWarning) continue;
        if (log.type == LogEntry::LOG_ERROR && !m_filterError) continue;
        if (log.type == LogEntry::LOG_DEBUG && !m_filterDebug) continue;

        ImVec4 color;
        const char* prefix = "[LOG]";
        switch (log.type) {
            case LogEntry::LOG_INFO:
                color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
                prefix = "[INFO]";
                break;
            case LogEntry::LOG_WARNING:
                color = ImVec4(1.0f, 0.85f, 0.0f, 1.0f);
                prefix = "[WARNING]";
                break;
            case LogEntry::LOG_ERROR:
                color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                prefix = "[ERROR]";
                break;
            case LogEntry::LOG_DEBUG:
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
            Logger::getInstance().addLog(LogEntry::LOG_INFO, std::string("> ") + m_commandBuffer);
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
    auto newLogs = Logger::getInstance().flushLogs();
    for (const auto& log : newLogs) {
        m_logs.push_back(log);
    }
    if (!newLogs.empty()) {
        m_scrollToBottom = true;
    }
}

void ConsolePanel::clearLogs() {
    m_logs.clear();
}
