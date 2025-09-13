#include <windows.h>
#include <atomic>

#include "gui.h"
#include "render_interface.h"

// API to expose to DDrawLoader
static RenderPluginAPI pluginAPI = {
    gui::Init,
    gui::DrawGUI,
    gui::Update,
    gui::ExitRequested,
    nullptr  // optional
};

// DDrawLoader requires this function to be implemented to run the render logic
extern "C" __declspec(dllexport) RenderPluginAPI* GetRenderPlugin() {
    return &pluginAPI;
}

// Exit request state
static std::atomic<bool> shouldExit = false;

// Per-frame callback
void gui::Update() {
    if(GetAsyncKeyState(VK_HOME) & 1) { 
        gui::ToggleMenu();
    }
    if(GetAsyncKeyState(VK_END) & 1) {
        shouldExit = true;
    }
}

// Getter for exit request state
bool gui::ExitRequested() {
    return shouldExit;
}

// Cursor loaded from system
static HCURSOR hCursor = nullptr;

// Initialize the global menu state (menu hidden by default)
static bool isMenuVisible = false;

void gui::ToggleMenu() {
    isMenuVisible = !isMenuVisible;
    OutputDebugStringA("[+] Menu toggled!\n");
}

void gui::Init() {
    hCursor = LoadCursor(NULL, IDC_ARROW);
    OutputDebugStringA("[+] GUI initialized\n");
}

void gui::DrawGUI(HDC hdc) {
    if(isMenuVisible) {
        // Order is important - the last thing is drawn on top (and we want the cursor to be above everything else)
        DrawMenu(hdc);
        DrawCursor(hdc);
    }
}

void gui::DrawMenu(HDC hdc) {
    // Menu dimensions and position
    constexpr int menuX = 50;
    constexpr int menuY = 50;
    constexpr int menuWidth = 200;
    constexpr int menuHeight = 150;

    // Draw filled background (light gray for now)
    HBRUSH bgBrush = CreateSolidBrush(RGB(240, 240, 240));
    RECT menuRect = { menuX, menuY, menuX + menuWidth, menuY + menuHeight };
    FillRect(hdc, &menuRect, bgBrush);
    DeleteObject(bgBrush);

    // Draw border (dark gray)
    HPEN borderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HGDIOBJ oldPen = SelectObject(hdc, borderPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH)); // No fill

    Rectangle(hdc, menuX, menuY, menuX + menuWidth, menuY + menuHeight);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(borderPen);

    // Draw some menu text
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));  // Black text

    TextOutA(hdc, menuX + 10, menuY + 10, "Option 1", strlen("Option 1"));
    TextOutA(hdc, menuX + 10, menuY + 30, "Option 2", strlen("Option 2"));
    TextOutA(hdc, menuX + 10, menuY + 50, "Option 3", strlen("Option 3"));
}

// Draw mouse cursor to operate the menu 
void gui::DrawCursor(HDC hdc) {
    // Get the foremost window (including full-screen)
    HWND hwnd = GetForegroundWindow();
    if(!hwnd) {
        OutputDebugStringA("[!] GetForegroundWindow failed\n");
        return;
    }

    // Get the current mouse cursor position to track user's movement
    POINT pt;
    if(!GetCursorPos(&pt)) {
        OutputDebugStringA("[!] GetCursorPos failed\n");
        return;
    }

    // Convert screen coordinates to client coordinates
    if(!ScreenToClient(hwnd, &pt)) {
        OutputDebugStringA("[!] ScreenToClient failed\n");
        return;
    }

    // Real/current cursor
    CURSORINFO ci = { 0 };
    ci.cbSize = sizeof(ci);

    // Try to capture the real/current cursor
    if(GetCursorInfo(&ci) && (ci.flags & CURSOR_SHOWING)) {
        DrawIcon(hdc, pt.x, pt.y, ci.hCursor);
        OutputDebugStringA("[*] Drew system cursor!\n");
        
    }
    // Fallback #1 - draw a loaded cursor icon
    else if(hCursor) {
        DrawIcon(hdc, pt.x, pt.y, hCursor);
        OutputDebugStringA("[*] Drew default arrow cursor manually!\n");
    }
    // Fallback #2 - draw a custom cursor
    else {
        // Draw a white crosshair at the cursor position
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
        HGDIOBJ oldPen = SelectObject(hdc, pen);

        MoveToEx(hdc, pt.x - 5, pt.y, nullptr);
        LineTo(hdc, pt.x + 5, pt.y);

        MoveToEx(hdc, pt.x, pt.y - 5, nullptr);
        LineTo(hdc, pt.x, pt.y + 5);

        SelectObject(hdc, oldPen);
        DeleteObject(pen);
        OutputDebugStringA("[*] Drew crosshair via fallback window!\n");
    }
}