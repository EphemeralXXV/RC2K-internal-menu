#pragma once

#include "Button.h"

#include "actions.h"

namespace debug_menu::components {
    std::shared_ptr<Button> BuildMenuButton(
        const std::wstring& text,
        const std::function<void()>& onClick,
        // Feedback sound on button click
        const std::function<void()>& clickSfx = actions::SpawnLittleTimeRemainingSound,
        int width = 120, int height = 26
    );
}