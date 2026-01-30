#include <windows.h>
#include <atomic>
#include <memory>

#include "gui.h"
#include "render_interface.h"

// Components
#include "debug_menu.h"

// GDIKit
#include "Root.h"
#include "Menu.h"

// Actions & game memory
#include "actions.h"

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
        actions::ResetAllEffects(); // Clean up any active effects before exiting
    }
}

// Poll exit request state
bool gui::ExitRequested() {
    return shouldExit;
}

// Cursor loaded from system
static HCURSOR hCursor = nullptr;

// Initialize shared menu UI pointer
static std::weak_ptr<Menu> menuRef;

void gui::Init() {
    hCursor = LoadCursor(NULL, IDC_ARROW);

    // Create root container for the GUI
    RECT winRect;
    GetClientRect(GetForegroundWindow(), &winRect);
    Root::Create(winRect.right - winRect.left, winRect.bottom - winRect.top);
    if(!Root::Get()) {
        OutputDebugStringA("[-] Failed to create root container!\n");
        return;
    }

    InitMenu();
    OutputDebugStringA("[+] GUI initialized\n");
}

void gui::InitMenu() {
    if(menuRef.lock()) return;
    auto menu = BuildDebugMenu();
    menuRef = menu;
}

void gui::ToggleMenu() {
    auto menu = menuRef.lock();
    if(!menu) return;

    menu->SetDisplayed(!menu->IsDisplayed());

    OutputDebugStringA("[+] Menu toggled!\n");
}

void gui::DrawGUI(HDC hdc) {
    auto root = Root::Get();
    if(!root) return;

    // Update root size to match window size if necessary
    RECT winRect;
    GetClientRect(GetForegroundWindow(), &winRect);
    if(root->GetWidth() != (winRect.right - winRect.left) ||
       root->GetHeight() != (winRect.bottom - winRect.top)) {
        root->SetSize(winRect.right - winRect.left, winRect.bottom - winRect.top);
    }
    OutputDebugStringA(("[+] Watermark x is " + std::to_string(root->GetWidth() - 100) + "\n").c_str());
    OutputDebugStringA(("[+] Watermark y is " + std::to_string(root->GetHeight() - 20) + "\n").c_str());

    // Order is important - the last thing is drawn on top
    // (and we want the cursor to be above everything else)
    root->InitRender(hdc);

    auto menu = menuRef.lock();
    if(!menu) return;
    if(!menu->IsDisplayed()) return;

    // Draw cursor only if Menu is enabled
    DrawCursor(hdc);
}

static void PollMouseAndFeed(POINT pt) {
    auto root = Root::Get();
    if(!root) return;

    // Persistent previous-button state (per-process static so it survives across frames)
    static bool wasLeftDown = false;

    // Feed hover (every frame)
    root->InitFeedMouseEvent({ MouseEventType::Move, pt, MouseButton::Left });

    // Edge-detect buttons (GetAsyncKeyState high bit = currently down)
    SHORT leftState = GetAsyncKeyState(VK_LBUTTON);
    bool leftDown = (leftState & 0x8000) != 0;
    if(leftDown && !wasLeftDown) {
        // Transition: up -> down
        root->InitFeedMouseEvent({ MouseEventType::Down, pt, MouseButton::Left });
    }
    else if(!leftDown && wasLeftDown) {
        // Transition: down -> up
        root->InitFeedMouseEvent({ MouseEventType::Up, pt, MouseButton::Left });
        root->InitFeedMouseEvent({ MouseEventType::Click, pt, MouseButton::Left });
    }
    wasLeftDown = leftDown;
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
    PollMouseAndFeed(pt);
}