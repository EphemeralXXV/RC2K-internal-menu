#pragma once

#include "Select.h"

namespace debug_menu::components {
    std::vector<Select::SelectItemPtr> BuildMenuSelectOptions(
        std::vector<std::pair<int, std::string>> optionsTable,
        std::function<void(int)> SetInGameValue
    );
}