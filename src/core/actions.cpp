#include "actions.h"
#include "game_memory.h"

void actions::RepairCar() {
    for(const auto& [name, addr_damage] : gameMemory::ADDR_DAMAGE) {
        OutputDebugStringA((name + " repaired.\n").c_str());
        gameMemory::mem<float>(addr_damage) = 0.00f;
    }
    gameMemory::mem<int>(gameMemory::ADDR_CAR_PARTS_LEFT) = -1;
    OutputDebugStringA("Car repaired.\n");
}
void actions::DestroyCar() {
    for(const auto& [name, addr_damage] : gameMemory::ADDR_DAMAGE) {
        OutputDebugStringA((name + " repaired.\n").c_str());
        gameMemory::mem<float>(addr_damage) = 100.00f;
    }
    gameMemory::mem<int>(gameMemory::ADDR_CAR_PARTS_LEFT) = 0;
    OutputDebugStringA("Car destroyed.\n");
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
int actions::GetCarID() {
    return gameMemory::mem<int>(gameMemory::ADDR_CAR_ID);
}
void actions::SetCarID(int id) {
    gameMemory::mem<int>(gameMemory::ADDR_CAR_ID) = id;
    OutputDebugStringA(("Car ID changed to " + std::to_string(id) + ".\n").c_str());
}

void actions::RestartStage() {
    gameMemory::mem<int>(gameMemory::ADDR_TRIGGER_RESTART_STAGE) = 1;
    OutputDebugStringA("Stage reset.\n");
}

void actions::SetBrakeBalance(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_BRAKE_BALANCE) = value;
    OutputDebugStringA(("Brake balance set to " + std::to_string(value) + ".\n").c_str());
}
void actions::ResetBrakeBalance() {
    gameMemory::mem<int>(gameMemory::ADDR_BRAKE_BALANCE) = gameMemory::DEFAULT_BRAKE_BALANCE;
    OutputDebugStringA("Brake balance reset.\n");
}

void actions::SetGearRatio(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_GEAR_RATIO) = value;
    OutputDebugStringA(("Gear ratio set to " + std::to_string(value) + ".\n").c_str());
}
void actions::ResetGearRatio() {
    gameMemory::mem<int>(gameMemory::ADDR_GEAR_RATIO) = gameMemory::DEFAULT_GEAR_RATIO;
    OutputDebugStringA("Gear ratio reset.\n");
}
int actions::GetGearRatio() {
    return gameMemory::mem<int>(gameMemory::ADDR_GEAR_RATIO);
}

void actions::SetTyreType(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_TYRE_TYPE) = value;
    OutputDebugStringA(("Tyre type set to " + std::to_string(value) + ".\n").c_str());
}
void actions::ResetTyreType() {
    gameMemory::mem<int>(gameMemory::ADDR_TYRE_TYPE) = gameMemory::DEFAULT_TYRE_TYPE;
    OutputDebugStringA("Tyre type reset.\n");
}
int actions::GetTyreType() {
    return gameMemory::mem<int>(gameMemory::ADDR_TYRE_TYPE);
}

void actions::SetSuspensionStiffness(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS) = value;
    OutputDebugStringA(("Suspension stiffness set to " + std::to_string(value) + ".\n").c_str());
}
void actions::ResetSuspensionStiffness() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_STIFFNESS) = gameMemory::DEFAULT_SUSPENSION_STIFFNESS;
    OutputDebugStringA("Suspension stiffness reset.\n");
}

void actions::SetSuspensionHeight(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_HEIGHT) = value;
    OutputDebugStringA(("Suspension height set to " + std::to_string(value) + "%.\n").c_str());
}
void actions::ResetSuspensionHeight() {
    gameMemory::mem<int>(gameMemory::ADDR_SUSPENSION_HEIGHT) = gameMemory::DEFAULT_SUSPENSION_HEIGHT;
    OutputDebugStringA("Suspension height reset.\n");
}

void actions::SetTransmissionType(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_TRANSMISSION_TYPE) = value;
    OutputDebugStringA(("Transmission type set to " + std::to_string(value) + ".\n").c_str());
}
void actions::ResetTransmissionType() {
    gameMemory::mem<int>(gameMemory::ADDR_TRANSMISSION_TYPE) = gameMemory::DEFAULT_TRANSMISSION_TYPE;
    OutputDebugStringA("Transmission type reset.\n");
}
int actions::GetTransmissionType() {
    return gameMemory::mem<int>(gameMemory::ADDR_TRANSMISSION_TYPE);
}

void actions::SetSteeringSensitivity(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_STEERING_SENSITIVITY) = value;
    OutputDebugStringA(("Steering sensitivity set to " + std::to_string(value) + ".\n").c_str());
}
void actions::ResetSteeringSensitivity() {
    gameMemory::mem<float>(gameMemory::ADDR_STEERING_SENSITIVITY) = gameMemory::DEFAULT_STEERING_SENSITIVITY;
    OutputDebugStringA("Steering sensitivity reset.\n");
}

void actions::SetGravity(float value) {
    gameMemory::mem<float>(gameMemory::ADDR_GRAVITY) = value;
    OutputDebugStringA(("Gravity set to " + std::to_string(value) + " m/s^2.\n").c_str());
}
void actions::ResetGravity() {
    gameMemory::mem<float>(gameMemory::ADDR_GRAVITY) = gameMemory::DEFAULT_GRAVITY;
    OutputDebugStringA("Gravity reset.\n");
}

// Accepts value expressed in km/h, but the game stores it in m/s -- needs conversion (divide by 3.6)
void actions::SetMaxResetSpeed(float value) {
    gameMemory::mem<float>(gameMemory::ADDR_MAX_RESET_SPEED) = value / 3.6f;
    OutputDebugStringA(("Max reset speed set to " + std::to_string(value) + " m/s.\n").c_str());
}
void actions::ResetMaxResetSpeed() {
    gameMemory::mem<float>(gameMemory::ADDR_MAX_RESET_SPEED) = gameMemory::DEFAULT_MAX_RESET_SPEED * 3.6f;
    OutputDebugStringA("Max reset speed reset.\n");
}

void actions::SetMaxDrawDistance(int value) {
    gameMemory::mem<int>(gameMemory::ADDR_MAX_DRAW_DISTANCE) = value;
    OutputDebugStringA(("Max draw distance set to " + std::to_string(value) + ".\n").c_str());
}
void actions::ResetMaxDrawDistance() {
    gameMemory::mem<int>(gameMemory::ADDR_MAX_DRAW_DISTANCE) = gameMemory::DEFAULT_MAX_DRAW_DISTANCE;
    OutputDebugStringA("Max draw distance reset.\n");
}

void actions::SetReflections(bool enabled) {
    gameMemory::mem<int>(gameMemory::ADDR_REFLECTIONS) = static_cast<int>(enabled);
    OutputDebugStringA(("Reflections " + std::string(enabled ? "enabled" : "disabled") + ".\n").c_str());
}
void actions::ResetReflections() {
    gameMemory::mem<int>(gameMemory::ADDR_REFLECTIONS) = gameMemory::DEFAULT_REFLECTIONS;
    OutputDebugStringA("Reflections reset.\n");
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
void actions::SpawnOutOfTimeSound() {
    gameMemory::mem<int>(gameMemory::ADDR_TRIGGER_OUT_OF_TIME_SOUND) = 1;
    OutputDebugStringA("Out of time!\n");
}

void actions::Retire() {
    gameMemory::mem<int>(gameMemory::ADDR_TIME_STOPPED_CAR_TO_LOSS) = 1;
    OutputDebugStringA("You lose!\n");
}
void actions::Win() {
    gameMemory::mem<int>(gameMemory::ADDR_TIME_STOPPED_CAR_TO_WIN) = 1;
    OutputDebugStringA("You win!\n");
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