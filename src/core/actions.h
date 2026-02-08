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

    int GetCarID();
    void SetCarID(int id);

    void SetBrakeBalance(int value);
    void ResetBrakeBalance();

    int GetGearRatio();
    void SetGearRatio(int value);
    void ResetGearRatio();

    int GetTyreType();
    void SetTyreType(int value);
    void ResetTyreType();

    void SetSuspensionStiffness(int value);
    void ResetSuspensionStiffness();

    void SetSuspensionHeight(int value);
    void ResetSuspensionHeight();

    int GetTransmissionType();
    void SetTransmissionType(int value);
    void ResetTransmissionType();

    void SetSteeringSensitivity(int value);
    void ResetSteeringSensitivity();
    
    void SetGravity(float value);
    void ResetGravity();

    void SetMaxResetSpeed(float value);
    void ResetMaxResetSpeed();

    void SetMaxDrawDistance(int value);
    void ResetMaxDrawDistance();

    void SetReflections(bool enabled);
    void ResetReflections();

    void SpawnLightningSound();
    void SpawnCheckpointSound();
    void SpawnOutOfTimeSound();
    void SpawnLittleTimeRemainingSound();

    void RestartStage();
    void Retire();
    void Win();
    void SetGameOverFlag(bool set);

    void ResetAllEffects();

    inline FreezeManager<float> floatFreezer;
}