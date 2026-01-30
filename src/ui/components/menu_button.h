#pragma once

#include "Button.h"

std::shared_ptr<Button> BuildMenuButton(
    const std::wstring& text,
    const std::function<void()>& onClick,
    int width = 120, int height = 26
);