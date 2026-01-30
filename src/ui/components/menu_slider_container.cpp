#include "menu_slider_container.h"

#include "Button.h"
#include "Slider.h"
#include "FlexLayout.h"

std::shared_ptr<Container> BuildMenuSliderContainer(
    const std::wstring& label,
    float min, float max, float step, float defaultValue,
    std::function<void(float)> onChange,
    std::function<void()> onReset,
    std::function<float()> currentValue,
    int sliderWidth, int sliderHeight,
    int btnWidth, int btnHeight
) {
    auto container = std::make_shared<Container>();
    auto sliderLayout = std::make_unique<FlexLayout>(FlexDirection::Row, 4);
    sliderLayout->SetAlign(AlignItems::Center);
    sliderLayout->SetJustify(JustifyContent::Center);
    container->SetLayout(std::move(sliderLayout));
    container->SetAutoWidth(true);
    container->SetAutoHeight(true);

    auto slider = std::make_shared<Slider>(label, min, max, step, defaultValue);
    slider->SetOnValueChanged([onChange](float val) {
        onChange(val);
    });
    if(currentValue) {
        slider->SetOnRender([slider, currentValue] {
            float newVal = currentValue();
            if(slider->GetValue() != newVal) {
                 // Sync slider position if value changed in-game
                slider->SetValue(newVal);
            }
        });
    }
    slider->SetSize(sliderWidth, sliderHeight);

    auto btnReset = std::make_shared<Button>(L"Reset");
    btnReset->SetOnClick([onReset, slider, defaultValue]() {
        onReset();
        slider->SetValue(defaultValue); // Also reset slider position
    });
    btnReset->SetSize(btnWidth, btnHeight);

    container->AddChild(slider);
    container->AddChild(btnReset);

    return container;
}