#include "ConsolePanel.h"

#include <queue>
#include <string>
#include <vector>

#include <imgui.h>

#include "Core/EventSystem/EventSystem.h"
#include "Core/Logger/Logger.h"
#include "Systems/UI/Theme/EditorTheme.h"

void ConsolePanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    updateLogsFromBuffer();

    applyPanelRect(layout.console);
    ImGui::Begin("##Console", nullptr, EDITOR_PANEL_WINDOW_FLAGS);
    EditorTheme::drawPanelHeader("Terminal", "Output Log");

    // Count logs for filter badges
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    int debug_count = 0;

    for (const LogEntry& log_entry : m_logs) {
        switch (log_entry.type) {
        case LogType::LOG_INFO:
            ++info_count;
            break;
        case LogType::LOG_WARNING:
            ++warning_count;
            break;
        case LogType::LOG_ERROR:
            ++error_count;
            break;
        case LogType::LOG_DEBUG:
            ++debug_count;
            break;
        }
    }

    if (ImGui::Button("Clear")) {
        EditorEvent clear_event;
        clear_event.type = EditorEventType::ClearConsole;
        EventSystem::pushEvent(clear_event);
        clearLogs();
    }

    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &m_autoScroll);

    ImGui::SameLine();
    std::string info_label = "Info (" + std::to_string(info_count) + ")";
    ImGui::Checkbox(info_label.c_str(), &m_filterInfo);

    ImGui::SameLine();
    std::string warning_label = "Warning (" + std::to_string(warning_count) + ")";
    ImGui::Checkbox(warning_label.c_str(), &m_filterWarning);

    ImGui::SameLine();
    std::string error_label = "Error (" + std::to_string(error_count) + ")";
    ImGui::Checkbox(error_label.c_str(), &m_filterError);

    ImGui::SameLine();
    std::string debug_label = "Debug (" + std::to_string(debug_count) + ")";
    ImGui::Checkbox(debug_label.c_str(), &m_filterDebug);

    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    const float footer_height = ImGui::GetFrameHeightWithSpacing() + 4.0f;
    ImGui::BeginChild("LogScrollRegion", ImVec2(0.0f, -footer_height), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    for (const LogEntry& log_entry : m_logs) {
        if (log_entry.type == LogType::LOG_INFO && !m_filterInfo) {
            continue;
        }
        if (log_entry.type == LogType::LOG_WARNING && !m_filterWarning) {
            continue;
        }
        if (log_entry.type == LogType::LOG_ERROR && !m_filterError) {
            continue;
        }
        if (log_entry.type == LogType::LOG_DEBUG && !m_filterDebug) {
            continue;
        }

        ImVec4 text_color;
        const char* prefix = "[LOG]";

        switch (log_entry.type) {
        case LogType::LOG_INFO:
            text_color = ImVec4(0.85f, 0.90f, 0.96f, 1.0f);
            prefix = "[INFO]";
            break;
        case LogType::LOG_WARNING:
            text_color = ImVec4(1.0f, 0.82f, 0.28f, 1.0f);
            prefix = "[WARN]";
            break;
        case LogType::LOG_ERROR:
            text_color = ImVec4(1.0f, 0.38f, 0.38f, 1.0f);
            prefix = "[ERROR]";
            break;
        case LogType::LOG_DEBUG:
            text_color = ImVec4(0.60f, 0.76f, 0.90f, 1.0f);
            prefix = "[DEBUG]";
            break;
        }

        // Special highlight for commands typed by the user
        if (log_entry.message.rfind("> ", 0) == 0) {
            text_color = ImVec4(0.40f, 0.92f, 0.62f, 1.0f);
            prefix = "[CMD]";
        }

        ImGui::PushStyleColor(ImGuiCol_Text, text_color);
        ImGui::TextWrapped("%-7s %s", prefix, log_entry.message.c_str());
        ImGui::PopStyleColor();
    }

    if (m_scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        m_scrollToBottom = false;
    }

    ImGui::EndChild();

    bool reclaim_focus = false;
    constexpr ImGuiInputTextFlags INPUT_FLAGS = ImGuiInputTextFlags_EnterReturnsTrue;
    ImGui::SetNextItemWidth(-1.0f);
    if (ImGui::InputTextWithHint("##ConsoleInput", "Enter console command...", m_commandBuffer, sizeof(m_commandBuffer), INPUT_FLAGS)) {
        if (m_commandBuffer[0] != '\0') {
            const std::string command = m_commandBuffer;

            EditorEvent cmd_event;
            cmd_event.type = EditorEventType::SubmitConsoleCommand;
            cmd_event.info.message = command;
            EventSystem::pushEvent(cmd_event);

            Logger::addLog(LogType::LOG_INFO, std::string("> ") + command);
            m_commandBuffer[0] = '\0';
            reclaim_focus = true;
            m_scrollToBottom = true;
        }
    }

    if (reclaim_focus) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}

void ConsolePanel::updateLogsFromBuffer() {
    std::queue<LogEntry> new_logs_queue = Logger::getLogs();
    const bool had_new_logs = !new_logs_queue.empty();

    while (!new_logs_queue.empty()) {
        m_logs.push_back(new_logs_queue.front());
        new_logs_queue.pop();
    }

    if (had_new_logs && m_autoScroll) {
        m_scrollToBottom = true;
    }
}

void ConsolePanel::clearLogs() {
    m_logs.clear();
    Logger::clearLogs();
}
