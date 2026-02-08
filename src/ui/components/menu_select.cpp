#include "menu_select.h"

#include "Label.h"
#include "FlexLayout.h"

std::shared_ptr<Select> debug_menu::components::BuildMenuSelect(
    std::vector<Select::SelectItemPtr> options,
    const std::function<std::string()> GetInGameValue,
    int width, int height
) {
    auto select = std::make_shared<Select>();
    select->SetItems(options);
    select->SetOnRender([select, GetInGameValue]() {
        // Poll current value to keep selection up to date
        static std::string lastValue;
        std::string currentValue = GetInGameValue();
        
        if(currentValue == lastValue) return;

        // Find the item with matching value
        // Can't simply select by raw value because it might not match the select item index
        auto& selectItems = select->GetItems();
        for(size_t i = 0; i < selectItems.size(); i++) {
            std::string value = selectItems[i]->GetValue();
            if(value == currentValue) {
                select->SetSelectedIndex(i);
                lastValue = currentValue; // Update last selection
                break;
            }
        }
    });
    select->SetSize(150, 24);

    return select;
}

std::shared_ptr<Container> debug_menu::components::BuildLabeledMenuSelect(
    std::vector<Select::SelectItemPtr> options,
    const std::function<std::string()> GetInGameValue,
    std::wstring labelText,
    int width, int height
) {
    auto container = std::make_shared<Container>();
    container->SetAutoHeight(true);
    container->SetAutoWidth(true);
    container->SetLayout(std::make_unique<VerticalLayout>(1));

    auto label = std::make_shared<Label>(labelText);

    auto select = BuildMenuSelect(options, GetInGameValue, width, height);

    container->AddChild(label);
    container->AddChild(select);

    return container;
}