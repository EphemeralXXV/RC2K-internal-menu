#include "menu_button.h"

std::shared_ptr<Button> debug_menu::components::BuildMenuButton(
    const std::wstring& text,
    const std::function<void()>& onClick,
    const std::function<void()>& clickSfx,
    int width, int height
) {
    auto button = std::make_shared<Button>(text);
    button->SetOnClick([onClick, clickSfx] {
        if(clickSfx) {
            clickSfx();
        }
        if(onClick) {
            onClick();
        }
    });
    button->SetSize(width, height);
    return button;
}