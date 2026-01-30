#include "actions.h"
#include "game_memory.h"

#include "Menu.h"
#include "Root.h"
#include "FlexLayout.h"
#include "Checkbox.h"
#include "Label.h"
#include "Select.h"

#include "debug_menu.h"
#include "menu_button.h"
#include "menu_slider_container.h"

std::shared_ptr<Menu> BuildDebugMenu() {
    auto root = Root::Get();
    if(!root) return nullptr;

    // Add watermark as an anti-cheat measure
    auto watermark = std::make_shared<Label>(L"CrashBoard v1.1");
    watermark->SetAnchor(Anchor::BottomRight);
    watermark->SetPosSize(10, 10, 85, 20);
    watermark->SetTextColor(Color::FromARGB(150, 255, 255, 255));
    root->AddChild(watermark);

    // Configure the menu itself
    auto menu = std::make_shared<Menu>(L"CrashBoard v1.1");
    menu->SetPosSize(40, 40, 300, 300);
    menu->SetBodyPadding(4, 4);
    menu->SetDisplayed(false);
    root->AddChild(menu);

    // Create children widgets
    auto btnRepair = BuildMenuButton(L"Repair car", []() {
        actions::RepairCar();
    });
    auto btnCheck = BuildMenuButton(L"Play checkpoint sound", []() {
        actions::SpawnCheckpointSound();
    });
    auto btnBoom = BuildMenuButton(L"Play lightning sound", []() {
        actions::SpawnLightningSound();
    });

    auto cbInvincibility = std::make_shared<Checkbox>(L"Invincibility");
    cbInvincibility->SetOnToggle([](bool checked) {
        if(checked) {
            actions::EnableInvincibility();
        }
        else {
            actions::DisableInvincibility();
        }
    });
    cbInvincibility->SetSize(150, 20);

    /* Gravity */
    auto gravityContainer = BuildMenuSliderContainer(
        L"Gravity:",
        -50.0f, 50.0f, 0.1f, gameMemory::DEFAULT_GRAVITY,
        [](float val) {
            actions::SetGravity(val);
        },
        []() {
            actions::ResetGravity();
        }
    );

    /* Suspension height */
    auto suspensionHeightContainer = BuildMenuSliderContainer(
        L"Suspension Height:",
        0, 255, 1, gameMemory::DEFAULT_SUSPENSION_HEIGHT,
        [](float val) {
            actions::SetSuspensionHeight(val);
        },
        []() {
            actions::ResetSuspensionHeight();
        },
        []() {
            // onRender callback to show current suspension height value (sync if changed in-game)
            return static_cast<float>(gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_HEIGHT));
        }
    );

    /* Suspension stiffness */
    auto suspensionStiffnessContainer = BuildMenuSliderContainer(
        L"Suspension Stiffness:",
        0, 255, 1, gameMemory::DEFAULT_SUSPENSION_STIFFNESS,
        [](float val) {
            actions::SetSuspensionStiffness(val);
        },
        []() {
            actions::ResetSuspensionStiffness();
        },
        []() {
            // onRender callback to show current suspension stiffness value (sync if changed in-game)
            return static_cast<float>(gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS));
        }
    );

    /* Everything else */
    auto lbl = std::make_shared<Label>(L"Transmission:");
    lbl->SetSize(120, 20);

    auto select = std::make_shared<Select>();
    std::vector<Select::SelectItemPtr> options;
    options.push_back(std::make_shared<SelectItem>(L"Automatic", 0));
    options.push_back(std::make_shared<SelectItem>(L"Semi-automatic", 1));
    options.push_back(std::make_shared<SelectItem>(L"Manual", 2));
    select->SetItems(options);
    select->SetSize(150, 24);  // width x height of closed select box

    // Apply layout to menu and its children
    auto mainLayout = std::make_unique<VerticalLayout>(4);
    mainLayout->SetJustify(JustifyContent::Center);
    mainLayout->SetAlign(AlignItems::Center);
    menu->SetBodyLayout(std::move(mainLayout));

    // Add children to menu
    menu->AddBodyChild(btnCheck);
    menu->AddBodyChild(btnBoom);
    menu->AddBodyChild(btnRepair);
    menu->AddBodyChild(cbInvincibility);
    menu->AddBodyChild(gravityContainer);
    menu->AddBodyChild(suspensionHeightContainer);
    menu->AddBodyChild(suspensionStiffnessContainer);
    // menu->AddBodyChild(lbl);
    // menu->AddBodyChild(select);

    return menu;
}