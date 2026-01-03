#include <windows.h>
#include <atomic>
#include <memory>

#include "gui.h"
#include "render_interface.h"

// GDIKit
#include "Root.h"
#include "Menu.h"
#include "VerticalLayout.h"
#include "Button.h"
#include "Label.h"
#include "Checkbox.h"
#include "Slider.h"
#include "Select.h"

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

// Initialize shared menu UI pointer
static std::weak_ptr<Menu> menuRef;

void gui::Init() {
    hCursor = LoadCursor(NULL, IDC_ARROW);

    // Create root container for the GUI
    RECT winRect;
    GetClientRect(GetForegroundWindow(), &winRect);
    Root::Create(winRect.right, winRect.bottom);
    if(!Root::Get()) {
        OutputDebugStringA("[-] Failed to create root container!\n");
        return;
    }

    InitMenu();
    OutputDebugStringA("[+] GUI initialized\n");
}

void gui::InitMenu() {
    auto root = Root::Get();
    if(!root) return;
    if(menuRef.lock()) return;

    // Configure the menu itself
    auto menu = std::make_shared<Menu>(L"Menu");
    menu->SetPosSize(40, 40, 300, 300);
    menu->SetDisplayed(false);
    root->AddChild(menu);
    menuRef = menu;

    // Create children widgets
    auto btn = std::make_shared<Button>(L"Click me!");
    btn->SetOnClick([]() {
        OutputDebugStringA("Button clicked!");
    });
    btn->SetSize(120, 26);

    auto lbl = std::make_shared<Label>(L"Sample text");
    lbl->SetSize(120, 20);

    auto cb = std::make_shared<Checkbox>(L"Enable option");
    cb->SetOnToggle([](bool state) {
        OutputDebugStringA(state ? "[+] Checked\n" : "[-] Unchecked\n");
    });
    cb->SetSize(150, 20);

    auto slider = std::make_shared<Slider>(L"Slider value:", 0.0f, 100.0f, 1.0f, 50.0f);
    slider->SetOnValueChanged([](float val) {
        OutputDebugStringA(("Slider value: " + std::to_string(val) + "\n").c_str());
    });
    slider->SetSize(150, 35);

    auto select = std::make_shared<Select>();
    std::vector<Select::SelectItemPtr> options;
    options.push_back(std::make_shared<SelectItem>(L"Option 1", 0));
    options.push_back(std::make_shared<SelectItem>(L"Option 2", 1));
    options.push_back(std::make_shared<SelectItem>(L"Option 3", 2));
    select->SetItems(options);
    select->SetSize(150, 24);  // width x height of closed select box

    // Apply layout to menu and its children
    auto mainLayout = std::make_unique<VerticalLayout>(4);
    menu->SetBodyLayout(std::move(mainLayout));
    menu->AddBodyChild(btn);
    menu->AddBodyChild(lbl);
    menu->AddBodyChild(cb);
    menu->AddBodyChild(slider);
    menu->AddBodyChild(select);
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

    // Order is important - the last thing is drawn on top
    // (and we want the cursor to be above everything else)
    root->Render(hdc);

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
    root->FeedMouseEvent({ MouseEventType::Move, pt, MouseButton::Left });

    // Edge-detect buttons (GetAsyncKeyState high bit = currently down)
    SHORT leftState = GetAsyncKeyState(VK_LBUTTON);
    bool leftDown = (leftState & 0x8000) != 0;
    if(leftDown && !wasLeftDown) {
        // Transition: up -> down
        root->FeedMouseEvent({ MouseEventType::Down, pt, MouseButton::Left });
    }
    else if(!leftDown && wasLeftDown) {
        // Transition: down -> up
        root->FeedMouseEvent({ MouseEventType::Up, pt, MouseButton::Left });
        root->FeedMouseEvent({ MouseEventType::Click, pt, MouseButton::Left });
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