#include "menu_button.h"

#include "actions.h"

std::shared_ptr<Button> BuildMenuButton(
    const std::wstring& text,
    const std::function<void()>& onClick,
    int width, int height
) {
    auto button = std::make_shared<Button>(text);
    button->SetOnClick([onClick] {
        actions::SpawnLittleTimeRemainingSound(); // Feedback sound on button click
        onClick();
    });
    button->SetSize(width, height);
    return button;
}