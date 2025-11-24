#pragma once

namespace gui {
    void Init();
    void Update();
    bool ExitRequested();
    
    void InitMenu();
    void ToggleMenu();

    void DrawGUI(HDC hdc);
    void DrawMenu(HDC hdc);
    void DrawCursor(HDC hdc);
}