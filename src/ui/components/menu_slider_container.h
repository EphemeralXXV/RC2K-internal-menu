#pragma once

#include "Container.h"

std::shared_ptr<Container> BuildMenuSliderContainer(
    const std::wstring& label,
    float min, float max, float step, float defaultValue,
    std::function<void(float)> onChange,
    std::function<void()> onReset,
    std::function<float()> currentValue = nullptr, // optional - get current value from game memory
    int sliderWidth = 150, int sliderHeight = 35,
    int btnWidth = 60, int btnHeight = 26
);