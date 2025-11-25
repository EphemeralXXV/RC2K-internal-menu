// Minimal immediate-mode-ish GDI GUI framework for use inside a DirectDraw/DirectDrawSurface hook.
// Single-file example providing: Widget base, Label, Button, Menu container, mouse handling, and usage notes.
// Designed to be rendered into a game backbuffer HDC (call Render(hdc) from your hooked Flip/GetDC path)

#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <cmath>

// --- Simple utility types -------------------------------------------------
struct Color {
    BYTE a, r, g, b;
    COLORREF toCOLORREF() const { return RGB(r, g, b); }
    static Color FromRGB(BYTE r, BYTE g, BYTE b) { return {255, r, g, b}; }
    static Color FromARGB(BYTE a, BYTE r, BYTE g, BYTE b) { return {a, r, g, b}; }
};

// --- Forward declarations ------------------------------------------------
struct Widget;
using WidgetPtr = std::shared_ptr<Widget>;

// --- Base Widget ---------------------------------------------------------
struct Widget {
    // Bounding rectangle relative to parent
    RECT rect{0, 0, 0, 0};

    // Convenient expressions of rect geometry
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    // Widget states
    bool visible = true;
    bool enabled = true;
    bool hovered = false;
    bool pressed = false;

    Widget* parent = nullptr;
    Widget() = default;
    virtual ~Widget() = default;

    // Absolute coordinate getters (relative => absolute)
    int AbsX() const { return parent ? parent->AbsX() + x : x; }
    int AbsY() const { return parent ? parent->AbsY() + y : y; }
    int AbsRight() const { return AbsX() + width; }
    int AbsBottom() const { return AbsY() + height; }
    RECT AbsRect() const {
        return RECT{ AbsX(), AbsY(), AbsX() + width, AbsY() + height};
    }

    // Convenience getters and setters
    void UpdateConvenienceGeometry() {
        x = rect.left;
        y = rect.top;
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }
    virtual void UpdateInternalLayout() {
        // default: no-op. Derived widgets may override to update children elements.
    }
    void SetRect(int l, int t, int r, int b) {
        rect = {l, t, r, b};
        UpdateConvenienceGeometry();
        UpdateInternalLayout();
    }
    void SetPosSize(int x, int y, int w, int h) {
        rect = {x, y, x + w, y + h};
        UpdateConvenienceGeometry();
        UpdateInternalLayout();
    }
    
    // --- Mouse event handlers -----------------------------------------------------
    // Test if cursor currently over widget
    bool MouseInRect(POINT p) const {
        return p.x >= AbsX() && p.x <= AbsRight() &&
               p.y >= AbsY() && p.y <= AbsBottom();
    }
    virtual void OnMouseMove(POINT p) {
        hovered = MouseInRect(p);
    }
    virtual void OnMouseDown(POINT p) {
        if(!enabled) return;
        if(MouseInRect(p)) pressed = true;
    }
    virtual void OnMouseUp(POINT p) {
        if(!enabled) return;
        if(pressed && MouseInRect(p)) {
            // default: no-op. Derived widgets may override to trigger actions.
        }
        pressed = false;
    }

    virtual void Render(HDC hdc) {}
};

// --- Menu (container) ---------------------------------------------------
struct Menu : public Widget {
    std::vector<WidgetPtr> children;

    // Window state
    bool isCollapsed = false;
    bool isDragging = false;
    POINT dragOffset = {0,0};

    // Title bar
    std::wstring title = L"Menu";
    bool showTitleBar = true;
    int titleBarHeight = 22;

    // Appearance
    Color background = Color::FromARGB(180, 0, 0, 0);
    bool drawBackground = false;

    // Constructor
    Menu() {
        InitInternalElements();
    }

    // --- Child management --------------------------------------------------
    WidgetPtr titleBar;
    WidgetPtr closeButton;
    WidgetPtr collapseButton;
    
    void AddChild(const WidgetPtr &child) {
        child->parent = this;
        children.push_back(child);
    }
    void RemoveAll() { children.clear(); }
    
    // Create children immediately
    void InitInternalElements() {
        titleBar = std::make_shared<Widget>();
        AddChild(titleBar);

        closeButton = std::make_shared<Widget>();
        AddChild(closeButton);

        collapseButton = std::make_shared<Widget>();
        AddChild(collapseButton);
    }

    // Update children geometry dynamically
    void UpdateInternalLayout() override {
        titleBar->SetPosSize(0, 0, width, titleBarHeight);
        closeButton->SetPosSize(width - 20, 2, 18, 18);
        collapseButton->SetPosSize(width - 40, 2, 18, 18);

        // Propagate to child widgets if needed
        for(auto& c : children)
            c->UpdateInternalLayout();
    }

    // --- Rendering ---------------------------------------------------------
    void Render(HDC hdc) override {
        if(!visible)
            return;
        
        int saved = SaveDC(hdc);

        // --- Title bar ---
        if(showTitleBar) {
            HBRUSH barBrush = CreateSolidBrush(RGB(60, 60, 60));
            RECT titleBarRect = titleBar->AbsRect();
            FillRect(hdc, &titleBarRect, barBrush);
            DeleteObject(barBrush);

            // Bar border line
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(20,20,20));
            HGDIOBJ oldPen = SelectObject(hdc, pen);
            MoveToEx(hdc, AbsX(), AbsY() + titleBarHeight, NULL);
            LineTo(hdc, AbsX() + width, AbsY() + titleBarHeight);
            SelectObject(hdc, oldPen);
            DeleteObject(pen);

            // Title text
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(220,220,220));
            TextOutW(hdc, titleBar->AbsX() + 6, titleBar->AbsY() + 4, title.c_str(), (int)title.size());

            // Draw "X"
            RECT closeButtonRect = closeButton->AbsRect();
            DrawTextW(hdc, L"×", 1, &closeButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Draw collapse arrow ▾ or ▸
            WCHAR arrow = isCollapsed ? L'▸' : L'▾';
            RECT collapseButtonRect = collapseButton->AbsRect();
            DrawTextW(hdc, &arrow, 1, &collapseButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }

        // --- Draw menu background if expanded ---
        if(drawBackground && !isCollapsed) {
            HBRUSH br = CreateSolidBrush(background.toCOLORREF());
            RECT bg{ AbsX(), AbsY() + titleBarHeight, AbsX() + width, AbsY() + height };
            FillRect(hdc, &bg, br);
            DeleteObject(br);
        }

        // Render children in order (if menu is expanded)
        if(!isCollapsed) {
            for(auto &c : children) {
                if(c->visible) c->Render(hdc);
            }
        }

        RestoreDC(hdc, saved);
    }

    // --- Event forwarding --------------------------------------------------
    void OnMouseDown(POINT p) override {
        if(!visible) return;

        // Dragging via title bar
        RECT titleBarRect = titleBar->AbsRect();
        if(PtInRect(&titleBarRect, p)) {
            RECT closeButtonRect = closeButton->AbsRect();
            if(PtInRect(&closeButtonRect, p)) {
                // Close the window
                visible = false;
                return;
            }

            RECT collapseButtonRect = collapseButton->AbsRect();
            if(PtInRect(&collapseButtonRect, p)) {
                isCollapsed = !isCollapsed;
                return;
            }

            // Start dragging
            isDragging = true;
            dragOffset.x = p.x - AbsX(); // Must use absolute coords for mouse events
            dragOffset.y = p.y - AbsY();
            return;
        }

        // Default: forward to children if visible and not collapsed
        if(!isCollapsed) {
            for (auto it = children.rbegin(); it != children.rend(); ++it)
                (*it)->OnMouseDown(p);
        }
    }

    void OnMouseMove(POINT p) override {
        if(!visible) return;

        if(isDragging) {
            SetPosSize(p.x - dragOffset.x, p.y - dragOffset.y, width, height);
            return;
        }

        // Forward to children
        if(!isCollapsed) {
            for (auto it = children.rbegin(); it != children.rend(); ++it)
                (*it)->OnMouseMove(p);
        }
    }

    void OnMouseUp(POINT p) override {
        if(!visible) return;
        isDragging = false;

        // Forward to children
        if(!isCollapsed) {
            for (auto it = children.rbegin(); it != children.rend(); ++it)
                (*it)->OnMouseUp(p);
        }
    }
};

// --- Label ---------------------------------------------------------------
struct Label : public Widget {
    std::wstring text;
    HFONT font = nullptr; // optional custom font
    Color textColor = Color::FromRGB(255,255,255);

    Label(const std::wstring &t = L"") : text(t) {}

    void Render(HDC hdc) override {
        if (!visible) return;

        int saved = SaveDC(hdc);
        RECT r = AbsRect();

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, textColor.toCOLORREF());
        HFONT old = (HFONT)SelectObject(hdc, font ? font : (HFONT)GetStockObject(DEFAULT_GUI_FONT));

        DrawTextW(hdc, text.c_str(), (int)text.size(), &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        SelectObject(hdc, old);
        RestoreDC(hdc, saved);
    }

    // labels accept no input
    void OnMouseDown(POINT) override {}
    void OnMouseUp(POINT) override {}
    void OnMouseMove(POINT) override {}
};

// --- Button --------------------------------------------------------------
struct Button : public Widget {
    std::wstring text;
    Color backColor = Color::FromARGB(200, 30, 30, 30);
    Color hoverColor = Color::FromARGB(220, 50, 50, 50);
    Color pressColor = Color::FromARGB(255, 20, 110, 220);
    Color borderColor = Color::FromRGB(0,0,0);
    Color textColor = Color::FromRGB(255,255,255);
    HFONT font = nullptr;

    std::function<void()> onClick;

    Button(const std::wstring &t = L"Button") : text(t) {}

    void Render(HDC hdc) override {
        if(!visible) return;

        int saved = SaveDC(hdc);
        RECT r = AbsRect();

        // background
        HBRUSH br;
        if(!enabled) br = CreateSolidBrush(RGB(120,120,120));
        else if(pressed) br = CreateSolidBrush(pressColor.toCOLORREF());
        else if(hovered) br = CreateSolidBrush(hoverColor.toCOLORREF());
        else br = CreateSolidBrush(backColor.toCOLORREF());
        FillRect(hdc, &r, br);
        DeleteObject(br);

        // border
        HPEN pen = CreatePen(PS_SOLID, 1, borderColor.toCOLORREF());
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, r.left, r.top, r.right, r.bottom);
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);

        // text
        SetBkMode(hdc, TRANSPARENT);
        HFONT old = (HFONT)SelectObject(hdc, font ? font : (HFONT)GetStockObject(DEFAULT_GUI_FONT));
        SetTextColor(hdc, textColor.toCOLORREF());
        DrawTextW(hdc, text.c_str(), (int)text.size(), &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        SelectObject(hdc, old);

        RestoreDC(hdc, saved);
    }

    void OnMouseDown(POINT p) override {
        if(!enabled) return;

        RECT r = AbsRect();
        if(PtInRect(&r, p)) {
            pressed = true;
        }
    }
    void OnMouseUp(POINT p) override {
        if(!enabled) return;

        RECT r = AbsRect();
        if(pressed && PtInRect(&r, p)) {
            // fire click
            if (onClick) onClick();
        }
        pressed = false;
    }
};

// --- Checkbox ------------------------------------------------------------
struct Checkbox : public Widget {
    bool checked = false;
    std::wstring text;
    
    Color boxColor      = Color::FromARGB(255, 50, 50, 50);
    Color checkColor    = Color::FromARGB(255, 20, 110, 220);
    Color hoverColor    = Color::FromARGB(255, 80, 80, 80);
    Color textColor     = Color::FromRGB(255, 255, 255);

    std::function<void(bool)> onToggle; // Called when checkbox is toggled

    Checkbox(const std::wstring& label = L"") : text(label) {}

    void Render(HDC hdc) override {
        if(!visible) return;
        int saved = SaveDC(hdc);

        RECT r = AbsRect();
        int boxSize = height; // square box same height as widget

        // Draw box background
        HBRUSH br = CreateSolidBrush(hovered ? hoverColor.toCOLORREF() : boxColor.toCOLORREF());
        RECT checkboxRect = RECT{r.left, r.top, r.left + boxSize, r.top + boxSize};
        FillRect(hdc, &checkboxRect, br);
        DeleteObject(br);

        // Draw checkmark if checked
        if(checked) {
            HBRUSH checkBr = CreateSolidBrush(checkColor.toCOLORREF());
            RECT checkRect = {r.left + 4, r.top + 4, r.left + boxSize - 4, r.top + boxSize - 4};
            FillRect(hdc, &checkRect, checkBr);
            DeleteObject(checkBr);
        }

        // Draw label text
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, textColor.toCOLORREF());
        HFONT oldFont = (HFONT)SelectObject(hdc, (HFONT)GetStockObject(DEFAULT_GUI_FONT));
        RECT textRect = { r.left + boxSize + 4, r.top, r.right, r.bottom };
        DrawTextW(hdc, text.c_str(), (int)text.size(), &textRect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
        SelectObject(hdc, oldFont);

        RestoreDC(hdc, saved);
    }

    void OnMouseDown(POINT p) override {
        if(!enabled) return;
        if(MouseInRect(p)) pressed = true;
    }

    void OnMouseUp(POINT p) override {
        if(!enabled) return;

        // Toggle checked only if mouse up while over the checkbox
        if(pressed && MouseInRect(p)) {
            checked = !checked;
            if (onToggle) onToggle(checked); // Fire user-provided callback
        }
        pressed = false;
    }
};

// --- Slider ------------------------------------------------------------
struct Slider : public Widget {
    float minValue = 0.0f;
    float maxValue = 1.0f;
    float value = 0.5f;
    float step = 0.0f; // 0 means free movement

    std::wstring label;
    bool showValue = true;

    int handleWidth = 10;

    Color trackColor = Color::FromRGB(100, 100, 100);
    Color handleColor = Color::FromRGB(180, 180, 180);
    Color hoverColor = Color::FromRGB(220, 220, 220);
    Color dragColor = Color::FromRGB(150, 150, 255);

    bool isDragging = false;
    std::function<void(float)> onValueChanged;

    Slider(
        const std::wstring& label, float minVal, float maxVal, float step, float val
    ) : label(label), minValue(minVal), maxValue(maxVal), step(step), value(val) {}

    // Compute handle rect in absolute coordinates
    RECT HandleRect() const {
        // Current handle position as a fraction of the whole slider
        float t = (value - minValue) / (maxValue - minValue);
        int x = AbsX() + int(t * (width - handleWidth));
        int y = AbsY();
        return RECT{x, y, x + handleWidth, y + height};
    }

    void Render(HDC hdc) override {
        if(!visible) return;

        int saved = SaveDC(hdc);

        // Track
        RECT track = {AbsX(), AbsY() + height/2 - 2, AbsX() + width, AbsY() + height/2 + 2};
        HBRUSH br = CreateSolidBrush(trackColor.toCOLORREF());
        FillRect(hdc, &track, br);
        DeleteObject(br);

        // Handle
        RECT hr = HandleRect();
        Color handleCol = handleColor;

        // Determine handle color based on state
        POINT p;
        GetCursorPos(&p);
        if(PtInRect(&hr, p)) { // Handle hovered
            handleCol = hoverColor;
        }
        if(isDragging) { // Dragging takes precendence over hovering
            handleCol = dragColor;
        }
        br = CreateSolidBrush(handleCol.toCOLORREF());
        FillRect(hdc, &hr, br);
        DeleteObject(br);

        // Compute the top text rect (one line above)
        HFONT oldFont = (HFONT)SelectObject(hdc, (HFONT)GetStockObject(DEFAULT_GUI_FONT));
        TEXTMETRIC tm;
        GetTextMetrics(hdc, &tm);
        int textHeight = tm.tmHeight;

        RECT absRect = AbsRect();
        RECT top = { absRect.left, absRect.top - textHeight - 2, absRect.right, absRect.top};

        // Left-aligned label
        DrawTextW(hdc, label.c_str(), -1, &top, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

        // Right-aligned numeric value
        if(showValue) {
            std::wstring val = std::to_wstring((int)value);
            DrawTextW(hdc, val.c_str(), -1, &top, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
        }

        SelectObject(hdc, oldFont);
        RestoreDC(hdc, saved);
    }

    void UpdateValueFromMouse(int mouseX) {
        // Calculate new value
        RECT r = AbsRect();
        int relX = mouseX - r.left - handleWidth / 2;
        float t = (float)relX / (float)(width - handleWidth);
        t = std::clamp(t, 0.0f, 1.0f);
        float newValue = minValue + t * (maxValue - minValue);

        // Snap to step
        if(step > 0.0f)
            newValue = minValue + step * std::round((newValue - minValue) / step);

        // Update value, fire callback
        if(newValue != value) {
            value = newValue;
            if(onValueChanged) onValueChanged(value);
        }
    }

    void OnMouseMove(POINT p) override {
        if(!enabled) return;

        if(isDragging) UpdateValueFromMouse(p.x);
    }

    void OnMouseDown(POINT p) override {
        if(!enabled) return;

        if(!MouseInRect(p)) return; // React also to clicks on the track itself
        isDragging = true;
        UpdateValueFromMouse(p.x);
    }

    void OnMouseUp(POINT p) override {
        if(!enabled) return;

        if(isDragging) {
            UpdateValueFromMouse(p.x);
            isDragging = false;
        }
    }
};