#include "menu_select_options.h"

#include "SelectItem.h"

std::vector<Select::SelectItemPtr> debug_menu::components::BuildMenuSelectOptions(
    std::vector<std::pair<int, std::string>> optionsTable,
    std::function<void(int)> SetInGameValue
) {
    std::vector<Select::SelectItemPtr> options;

    for(size_t i = 0; i < optionsTable.size(); i++) {
        auto option = optionsTable[i];
        int id = option.first;
        std::string label = option.second;
        auto item = std::make_shared<SelectItem>(
            std::wstring(label.begin(), label.end()),
            std::to_string(id)
        );
        item->SetOnSelect([id, SetInGameValue]() {
            SetInGameValue(id);
        });
        options.push_back(item);
    }

    return options;
}