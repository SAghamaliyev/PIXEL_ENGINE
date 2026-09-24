#pragma once

#include "../Materials/imgui.h"

namespace EditorTheme {
    void setDark();
    void setBlue();
    void setPurple();

    const ImVec4& accent();
    const ImVec4& accentHover();
    const ImVec4& accentActive();

    void drawPanelHeader(const char* title, const char* subtitle = nullptr);
}
