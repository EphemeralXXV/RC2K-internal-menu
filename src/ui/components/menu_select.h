#pragma once

#include "Select.h"
#include "Container.h"

namespace debug_menu::components {
    std::shared_ptr<Select> BuildMenuSelect(
        std::vector<Select::SelectItemPtr> options,
        const std::function<std::string()> GetInGameValue,
        int width = 150, int height = 24
    );

    std::shared_ptr<Container> BuildLabeledMenuSelect(
        std::vector<Select::SelectItemPtr> options,
        const std::function<std::string()> GetInGameValue,
        std::wstring labelText = L"",
        int width = 150, int height = 24
    );
}