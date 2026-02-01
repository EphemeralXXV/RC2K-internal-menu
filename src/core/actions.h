#pragma once

#include <windows.h>
#include <map>
#include <functional>
#include <vector>
#include <thread>

#include "freeze_manager.h"

namespace actions {
    void RepairCar();
    void DestroyCar();
    void EnableInvincibility();
    void DisableInvincibility();

    void SetSuspensionStiffness(int value);
    void IncreaseSuspensionStiffness();
    void DecreaseSuspensionStiffness();
    void ResetSuspensionStiffness();

    void SetSuspensionHeight(int value);
    void IncreaseSuspensionHeight();
    void DecreaseSuspensionHeight();
    void ResetSuspensionHeight();
    
    void SetGravity(float value);
    void IncreaseGravity();
    void DecreaseGravity();
    void ResetGravity();

    void SpawnLightningSound();
    void SpawnCheckpointSound();
    void SpawnLittleTimeRemainingSound();

    void SetGameOverFlag(bool set);

    void ResetAllEffects();

    inline FreezeManager<float> floatFreezer;
}