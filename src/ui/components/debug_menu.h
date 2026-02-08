#pragma once
#include "Menu.h"

namespace debug_menu {
    std::shared_ptr<Menu> BuildDebugMenu();
    std::shared_ptr<Container> BuildCarDamageSection();
    std::shared_ptr<Container> BuildCarSetupSection();
    std::shared_ptr<Container> BuildDiagnosticsSection();
    std::shared_ptr<Container> BuildGameLogicSection();
    std::shared_ptr<Container> BuildGameSettingsSection();
    std::shared_ptr<Container> BuildMiscSection();
}