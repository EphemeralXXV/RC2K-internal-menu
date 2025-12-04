#include <windows.h>
#include <atomic>
#include <memory>

#include "gui.h"
#include "render_interface.h"
#include "Menu.h"
#include "Button.h"
#include "Label.h"
#include "Checkbox.h"
#include "Slider.h"

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

// Initialize shared menu UI pointer
static std::shared_ptr<Menu> menuUI;

void gui::Init() {
    hCursor = LoadCursor(NULL, IDC_ARROW);
    InitMenu();
    OutputDebugStringA("[+] GUI initialized\n");
}

void gui::InitMenu() {
    if(menuUI) return;

    // Configure the menu itself
    menuUI = std::make_shared<Menu>(L"Menu");
    menuUI->SetPosSize(10, 10, 300, 200);
    menuUI->SetDrawBackground(true);
    menuUI->SetVisible(isMenuVisible);

    // Create children widgets
    auto btn = std::make_shared<Button>(L"Click me!");
    btn->SetOnClick([]() {
        // MessageBoxW(nullptr, L"Button clicked!", L"Info", MB_OK);
    });
    btn->SetPreferredSize(120, 26);

    auto lbl = std::make_shared<Label>(L"Sample text");
    lbl->SetPreferredSize(120, 18);

    auto cb = std::make_shared<Checkbox>(L"Enable option");
    cb->SetOnToggle([](bool state) {
        OutputDebugStringA(state ? "[+] Checked\n" : "[-] Unchecked\n");
    });
    cb->SetPreferredSize(150, 20);

    auto slider = std::make_shared<Slider>(L"Slider value:", 0.0f, 100.0f, 1.0f, 50.0f);
    slider->SetOnValueChanged([](float val) {
        OutputDebugStringA(("Slider value: " + std::to_string(val) + "\n").c_str());
    });
    slider->SetPreferredSize(150, 20);

    // Apply layout to menu and its children
    menuUI->BeginLayout(10, 10);
        menuUI->AddChildWithLayout(btn);
        menuUI->AddChildWithLayout(lbl);
        menuUI->AddChildWithLayout(cb);
        menuUI->AddChildWithLayout(slider);
    menuUI->EndLayout();
}

void gui::ToggleMenu() {
    if(menuUI) {
        menuUI->SetVisible(!menuUI->IsVisible());
        isMenuVisible = menuUI->IsVisible();
    }

    OutputDebugStringA("[+] Menu toggled!\n");
}

void gui::DrawGUI(HDC hdc) {
    if(isMenuVisible) {
        // Order is important - the last thing is drawn on top
        // (and we want the cursor to be above everything else)
        DrawMenu(hdc);
        DrawCursor(hdc);
    }
}

void gui::DrawMenu(HDC hdc) {
    if(!menuUI) return;
    menuUI->Render(hdc);
}

static void PollMouseAndFeed(POINT pt) {
    // Persistent previous-button state (per-process static so it survives across frames)
    static bool wasLeftDown = false;
    static bool wasRightDown = false;
    static bool wasMiddleDown = false;

    // Feed hover (every frame)
    menuUI->FeedMouseEvent({ MouseEventType::Move, pt, 1 });

    // Edge-detect buttons (GetAsyncKeyState high bit = currently down)
    SHORT leftState = GetAsyncKeyState(VK_LBUTTON);
    bool leftDown = (leftState & 0x8000) != 0;
    if(leftDown && !wasLeftDown) {
        // Transition: up -> down
        menuUI->FeedMouseEvent({ MouseEventType::Down, pt, 1 });
    }
    else if(!leftDown && wasLeftDown) {
        // Transition: down -> up
        menuUI->FeedMouseEvent({ MouseEventType::Up, pt, 1 });
        menuUI->FeedMouseEvent({ MouseEventType::Click, pt, 1 });
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