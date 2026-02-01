#include "actions.h"
#include "game_memory.h"

void actions::RepairCar() {
    for(const auto& [name, addr_damage] : gameMemory::ADDR_DAMAGE) {
        OutputDebugStringA((name + " repaired.\n").c_str());
        gameMemory::mem<float>(addr_damage) = 0.00f;
    }
    OutputDebugStringA("Car repaired.\n");
}
void actions::DestroyCar() {
    for(const auto& [name, addr_damage] : gameMemory::ADDR_DAMAGE) {
        OutputDebugStringA((name + " repaired.\n").c_str());
        gameMemory::mem<float>(addr_damage) = 100.00f;
    }
    OutputDebugStringA("Car repaired.\n");
}
void actions::EnableInvincibility() {
    for(const auto& [name, addr_damage] : gameMemory::ADDR_DAMAGE) {
        floatFreezer.AddValue(addr_damage, 0.00f);
    }
    OutputDebugStringA("Invincibility ON.\n");
}
void actions::DisableInvincibility() {
    for(const auto& [name, addr_damage] : gameMemory::ADDR_DAMAGE) {
        floatFreezer.RemoveValue(addr_damage);
    }
    OutputDebugStringA("Invincibility OFF.\n");
}

void actions::SetSuspensionStiffness(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS) = value;
    OutputDebugStringA(("Suspension stiffness set to " + std::to_string(value) + ".\n").c_str());
}
void actions::IncreaseSuspensionStiffness() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS) += 1;
    OutputDebugStringA("Suspension stiffness increased by 1.\n");
}
void actions::DecreaseSuspensionStiffness() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS) -= 1;
    OutputDebugStringA("Suspension stiffness decreased by 1.\n");
}
void actions::ResetSuspensionStiffness() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS) = gameMemory::DEFAULT_SUSPENSION_STIFFNESS;
    OutputDebugStringA("Suspension stiffness reset.\n");
}

void actions::SetSuspensionHeight(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_HEIGHT) = value;
    OutputDebugStringA(("Suspension height set to " + std::to_string(value) + "%.\n").c_str());
}
void actions::IncreaseSuspensionHeight() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_HEIGHT) += 1;
    OutputDebugStringA("Suspension height increased by 1%.\n");
}
void actions::DecreaseSuspensionHeight() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_HEIGHT) -= 1;
    OutputDebugStringA("Suspension height decreased by 1%.\n");
}
void actions::ResetSuspensionHeight() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_HEIGHT) = gameMemory::DEFAULT_SUSPENSION_HEIGHT;
    OutputDebugStringA("Suspension height reset.\n");
}

void actions::SetGravity(float value) {
    gameMemory::mem<float>(gameMemory::ADDR_GRAVITY) = value;
    OutputDebugStringA(("Gravity set to " + std::to_string(value) + " m/s^2.\n").c_str());
}
void actions::IncreaseGravity() {
    gameMemory::mem<float>(gameMemory::ADDR_GRAVITY) += 1.00f;
    OutputDebugStringA("Gravity increased by 1 m/s^2.\n");
}
void actions::DecreaseGravity() {
    gameMemory::mem<float>(gameMemory::ADDR_GRAVITY) -= 1.00f;
    OutputDebugStringA("Gravity decreased by 1 m/s^2.\n");
}
void actions::ResetGravity() {
    gameMemory::mem<float>(gameMemory::ADDR_GRAVITY) = gameMemory::DEFAULT_GRAVITY;
    OutputDebugStringA("Gravity reset.\n");
}

void actions::SpawnLightningSound() {
    gameMemory::mem<int>(gameMemory::ADDR_LIGHTNING_SOUND_FLAG) = 1;
    OutputDebugStringA("Boom!\n");
}
void actions::SpawnCheckpointSound() {
    gameMemory::mem<int>(gameMemory::ADDR_TRIGGER_CHECKPOINT_SOUND) = 1;
    OutputDebugStringA("Checkpoint!\n");
}
void actions::SpawnLittleTimeRemainingSound() {
    gameMemory::mem<int>(gameMemory::ADDR_TRIGGER_LITTLE_TIME_REMAINING_SOUND) = 1;
    OutputDebugStringA("Little time remaining!\n");
}

void actions::SetGameOverFlag(bool set) {
    gameMemory::mem<int>(gameMemory::ADDR_GAME_OVER_FLAG) = set;
    OutputDebugStringA(("Game over flag set to " + std::to_string(set) + "!\n").c_str());
}

void actions::ResetAllEffects() {
    DisableInvincibility();
    ResetGravity();
    ResetSuspensionHeight();
    ResetSuspensionStiffness();
    
    OutputDebugStringA("All effects reset to default.\n");
}