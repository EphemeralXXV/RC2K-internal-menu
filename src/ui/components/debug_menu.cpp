#include "actions.h"
#include "game_memory.h"

#include "Menu.h"
#include "Root.h"
#include "FlexLayout.h"
#include "Checkbox.h"
#include "Label.h"
#include "Select.h"

#include "debug_menu.h"
#include "menu_section.h"
#include "menu_button.h"
#include "menu_select.h"
#include "menu_select_options.h"
#include "menu_slider_container.h"

std::shared_ptr<Menu> debug_menu::BuildDebugMenu() {
    auto root = Root::Get();
    if(!root) return nullptr;

    // Add watermark as an anti-cheat measure
    auto watermark = std::make_shared<Label>(L"CrashBoard Alpha v1.3");
    watermark->SetAnchor(Anchor::BottomRight);
    watermark->SetPos(10, 10);
    watermark->SetTextColor(Color::FromARGB(150, 255, 255, 255));
    root->AddChild(watermark);

    // Configure the menu itself
    auto menu = std::make_shared<Menu>(L"CrashBoard Alpha v1.3");
    menu->SetPosSize(10, 10, 300, 650);
    menu->SetBodyPadding(4, 4);
    menu->SetDisplayed(false);
    root->AddChild(menu);

    // Apply layout to menu and its children
    auto mainLayout = std::make_unique<VerticalLayout>(4);
    mainLayout->SetJustify(JustifyContent::Center);
    mainLayout->SetAlign(AlignItems::Center);
    menu->SetBodyLayout(std::move(mainLayout));

    // Add contents to menu
    menu->AddBodyChild(BuildCarDamageSection());
    menu->AddBodyChild(BuildCarSetupSection());
    menu->AddBodyChild(BuildDiagnosticsSection());
    menu->AddBodyChild(BuildGameLogicSection());
    menu->AddBodyChild(BuildGameSettingsSection());
    menu->AddBodyChild(BuildMiscSection());

    return menu;
}

std::shared_ptr<Container> debug_menu::BuildCarDamageSection() {
    auto dmgContainer = debug_menu::components::BuildMenuSection();

    auto btnRepair = debug_menu::components::BuildMenuButton(L"Repair car", []() {
        actions::RepairCar();
    });
    auto btnDestroy = debug_menu::components::BuildMenuButton(L"Destroy car", []() {
        actions::DestroyCar();
    }, actions::SpawnOutOfTimeSound);

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

    dmgContainer->AddChild(btnRepair);
    dmgContainer->AddChild(btnDestroy);
    dmgContainer->AddChild(cbInvincibility);

    return dmgContainer;
}

std::shared_ptr<Container> debug_menu::BuildCarSetupSection() {
    auto carSetupContainer = debug_menu::components::BuildMenuSection();

    /* Brake balance */
    auto brakeBalanceContainer = debug_menu::components::BuildMenuSliderContainer(
        L"Brake balance:",
        0, 255, 1, gameMemory::DEFAULT_BRAKE_BALANCE,
        [](float val) {
            actions::SetBrakeBalance(val);
        },
        []() {
            actions::ResetBrakeBalance;
        },
        []() {
            return static_cast<float>(gameMemory::mem<int>(gameMemory::ADDR_BRAKE_BALANCE));
        }
    );

    /* Suspension height */
    auto suspensionHeightContainer = debug_menu::components::BuildMenuSliderContainer(
        L"Suspension height:",
        0, 255, 1, gameMemory::DEFAULT_SUSPENSION_HEIGHT, // max value is actually MAX_INTEGER
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
    auto suspensionStiffnessContainer = debug_menu::components::BuildMenuSliderContainer(
        L"Suspension stiffness:",
        0, 255, 1, gameMemory::DEFAULT_SUSPENSION_STIFFNESS,
        [](float val) {
            actions::SetSuspensionStiffness(val);
        },
        []() {
            actions::ResetSuspensionStiffness();
        },
        []() {
            return static_cast<float>(gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS));
        }
    );

    /* Steering sensitivity */
    auto steeringSensitivityContainer = debug_menu::components::BuildMenuSliderContainer(
        L"Steering sensitivity:",
        0, 255, 1, gameMemory::DEFAULT_STEERING_SENSITIVITY,
        [](float val) {
            actions::SetSteeringSensitivity(val);
        },
        []() {
            actions::ResetSteeringSensitivity;
        },
        []() {
            return static_cast<float>(gameMemory::mem<int>(gameMemory::ADDR_STEERING_SENSITIVITY));
        }
    );

    /* Select car */
    std::vector<Select::SelectItemPtr> optionsSelectCar = debug_menu::components::BuildMenuSelectOptions(
        gameMemory::CARS,
        actions::SetCarID
    );
    auto selectCar = debug_menu::components::BuildLabeledMenuSelect(
        optionsSelectCar,
        []{ return std::to_string(actions::GetCarID()); },
        L"Car:"
    );

    /* Select Transmission type */
    std::vector<Select::SelectItemPtr> optionsSelectTransmissionType = debug_menu::components::BuildMenuSelectOptions(
        gameMemory::TRANSMISSION_TYPES,
        actions::SetTransmissionType
    );
    auto selectTransmissionType = debug_menu::components::BuildLabeledMenuSelect(
        optionsSelectTransmissionType,
        []{ return std::to_string(actions::GetTransmissionType()); },
        L"Transmission type:"
    );

    /* Select gear ratio */
    std::vector<Select::SelectItemPtr> optionsSelectGearRatio = debug_menu::components::BuildMenuSelectOptions(
        gameMemory::GEAR_RATIOS,
        actions::SetGearRatio
    );
    auto selectGearRatio = debug_menu::components::BuildLabeledMenuSelect(
        optionsSelectGearRatio,
        []{ return std::to_string(actions::GetGearRatio()); },
        L"Gear ratio:"
    );

    /* Select tyre type */
    std::vector<Select::SelectItemPtr> optionsSelectTyreType = debug_menu::components::BuildMenuSelectOptions(
        gameMemory::TYRE_TYPES,
        actions::SetTyreType
    );
    auto selectTyreType = debug_menu::components::BuildLabeledMenuSelect(
        optionsSelectTyreType,
        []{ return std::to_string(actions::GetTyreType()); },
        L"Tyre type:"
    );

    carSetupContainer->AddChild(brakeBalanceContainer);
    carSetupContainer->AddChild(suspensionHeightContainer);
    carSetupContainer->AddChild(suspensionStiffnessContainer);
    carSetupContainer->AddChild(steeringSensitivityContainer);
    carSetupContainer->AddChild(selectCar);
    carSetupContainer->AddChild(selectTransmissionType);
    carSetupContainer->AddChild(selectGearRatio);
    carSetupContainer->AddChild(selectTyreType);

    return carSetupContainer;
}

std::shared_ptr<Container> debug_menu::BuildDiagnosticsSection() {
    auto diagnosticsContainer = debug_menu::components::BuildMenuSection();

    HFONT labelFont = CreateFontW(
        12, 0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Verdana"
    );
    for(auto& pair : gameMemory::ADDR_DAMAGE) {
        std::wstring keyStr = std::wstring(pair.first.begin(), pair.first.end());
        auto label = std::make_shared<Label>(keyStr + L": " + std::to_wstring(gameMemory::mem<float>(pair.second)));
        label->SetOnRender([addr=pair.second, keyStr, label]() {
            label->SetText(keyStr + L": " + std::to_wstring(gameMemory::mem<float>(addr)));
        });
        label->SetFont(labelFont);
        diagnosticsContainer->AddChild(label);
    }

    return diagnosticsContainer;
}

std::shared_ptr<Container> debug_menu::BuildGameLogicSection() {
    auto gameLogicContainer = debug_menu::components::BuildMenuSection();

    // Create children widgets
    auto btnWin = debug_menu::components::BuildMenuButton(L"Win", []() {
        actions::Win();
    });
    auto btnRetire = debug_menu::components::BuildMenuButton(L"Retire", []() {
        actions::Retire();
    });
    auto btnRestart = debug_menu::components::BuildMenuButton(L"Restart", []() {
        actions::RestartStage();
    });

    /* Gravity */
    auto gravityContainer = debug_menu::components::BuildMenuSliderContainer(
        L"Gravity:",
        -50.0f, 50.0f, 0.1f, gameMemory::DEFAULT_GRAVITY,
        [](float val) {
            actions::SetGravity(val);
        },
        []() {
            actions::ResetGravity();
        }
    );

    /* Max reset speed */
    auto maxResetSpeedContainer = debug_menu::components::BuildMenuSliderContainer(
        L"Max reset speed (kph):",
        0.0f, 300.0f, 1.0f, gameMemory::DEFAULT_MAX_RESET_SPEED * 3.6,
        [](float val) {
            actions::SetMaxResetSpeed(val);
        },
        []() {
            actions::ResetMaxResetSpeed();
        },
        []() {
            return gameMemory::mem<float>(gameMemory::ADDR_MAX_RESET_SPEED) * 3.6;
        }
    );

    gameLogicContainer->AddChild(btnWin);
    gameLogicContainer->AddChild(btnRetire);
    gameLogicContainer->AddChild(btnRestart);
    gameLogicContainer->AddChild(gravityContainer);
    gameLogicContainer->AddChild(maxResetSpeedContainer);

    return gameLogicContainer;
}

std::shared_ptr<Container> debug_menu::BuildGameSettingsSection() {
    auto gameSettingsContainer = debug_menu::components::BuildMenuSection();

    auto cbReflections = std::make_shared<Checkbox>(L"Reflections");
    cbReflections->SetChecked(static_cast<bool>(gameMemory::DEFAULT_REFLECTIONS));
    cbReflections->SetOnToggle([](bool checked) {
        actions::SetReflections(checked);
    });
    cbReflections->SetSize(150, 20);

    /* Max draw distance */
    auto maxDrawDistanceContainer = debug_menu::components::BuildMenuSliderContainer(
        L"Max draw distance:",
        0, 16, 1, gameMemory::DEFAULT_MAX_DRAW_DISTANCE,
        [](int val) {
            actions::SetMaxDrawDistance(val);
        },
        []() {
            actions::ResetMaxDrawDistance();
        },
        []() {
            return gameMemory::mem<int>(gameMemory::ADDR_MAX_DRAW_DISTANCE);
        }
    );

    gameSettingsContainer->AddChild(cbReflections);
    gameSettingsContainer->AddChild(maxDrawDistanceContainer);

    return gameSettingsContainer;
}

std::shared_ptr<Container> debug_menu::BuildMiscSection() {
    auto miscContainer = debug_menu::components::BuildMenuSection();

    auto sfxContainer = std::make_shared<Container>();
    sfxContainer->SetAutoWidth(true);
    sfxContainer->SetAutoHeight(true);
    auto sfxContainerLayout = std::make_unique<FlexLayout>(FlexDirection::Row, 4);
    sfxContainerLayout->SetJustify(JustifyContent::Center);
    sfxContainer->SetLayout(std::move(sfxContainerLayout));

    auto btnCheck = debug_menu::components::BuildMenuButton(L"Play checkpoint sound", nullptr, actions::SpawnCheckpointSound);
    auto btnBoom = debug_menu::components::BuildMenuButton(L"Play lightning sound", nullptr, actions::SpawnLightningSound);
    sfxContainer->AddChild(btnCheck);
    sfxContainer->AddChild(btnBoom);

    miscContainer->AddChild(sfxContainer);

    return miscContainer;
}