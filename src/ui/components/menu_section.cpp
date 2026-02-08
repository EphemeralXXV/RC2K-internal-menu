#include "menu_section.h"

#include "FlexLayout.h"

std::shared_ptr<Container> debug_menu::components::BuildMenuSection() {
    auto section = std::make_shared<Container>();
    section->SetAutoWidth(true);
    section->SetAutoHeight(true);
    auto sectionLayout = std::make_unique<VerticalLayout>(4);
    sectionLayout->SetJustify(JustifyContent::Center);
    section->SetLayout(std::move(sectionLayout));

    return section;
}