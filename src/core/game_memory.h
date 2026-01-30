#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>
#include <atomic>

namespace gameMemory {

    // Helper function to access memory at a given address
    template<typename T>
    inline T& mem(uintptr_t addr) {
        return *reinterpret_cast<T*>(addr);
    }

    // Game memory addresses
    inline constexpr uintptr_t ADDR_GRAVITY                             = 0x00613fc8;
    inline constexpr uintptr_t ADDR_SUSPENSION_HEIGHT                   = 0x0070f41c;
    inline constexpr uintptr_t ADDR_SUSPENSION_STIFFNESS                = 0x0070f420;
    inline constexpr uintptr_t ADDR_LIGHTNING_SOUND_FLAG                = 0x005165a8;
    inline constexpr uintptr_t ADDR_TRIGGER_CHECKPOINT_SOUND            = 0x0051659c;
    inline constexpr uintptr_t ADDR_TRIGGER_LITTLE_TIME_REMAINING_SOUND = 0x005165a0;

    // Damage addresses
    inline std::unordered_map<std::string, uintptr_t> ADDR_DAMAGE = {
        {"ENGINE_DAMAGE",                   0x0070f6a4},
        {"EXHAUST_DAMAGE",                  0x0070f6a8},
        {"TURBO_DAMAGE",                    0x0070f6ac},
        {"COOLING_DAMAGE",                  0x0070f6b0},
        {"CLUTCH_DAMAGE",                   0x0070f6b4},
        {"DIFFERENTIAL_DAMAGE",             0x0070f6b8},
        {"REVERSE_GEAR_DAMAGE",             0x0070f6bc},
        {"NEUTRAL_GEAR_DAMAGE",             0x0070f6c0},
        {"FIRST_GEAR_DAMAGE",               0x0070f6c4},
        {"SECOND_GEAR_DAMAGE",              0x0070f6c8},
        {"THIRD_GEAR_DAMAGE",               0x0070f6cc},
        {"FOURTH_GEAR_DAMAGE",              0x0070f6d0},
        {"FIFTH_GEAR_DAMAGE",               0x0070f6d4},
        {"SIXTH_GEAR_DAMAGE",               0x0070f6d8},
        {"SEVENTH_GEAR_DAMAGE",             0x0070f6dc},
        {"FRONT_LEFT_WHEEL_DAMAGE",         0x0070f6e4},
        {"FRONT_RIGHT_WHEEL_DAMAGE",        0x0070f6e8},
        {"REAR_LEFT_WHEEL_DAMAGE",          0x0070f6ec},
        {"REAR_RIGHT_WHEEL_DAMAGE",         0x0070f6f0},
        {"BODYWORK_MAJOR_DAMAGE",           0x0070f6f4},
        {"ELECTRICS_DAMAGE",                0x0070f6fc},
        {"FRONT_LEFT_LIGHT_DAMAGE",         0x0070f700},
        {"FRONT_RIGHT_LIGHT_DAMAGE",        0x0070f704},
        {"REAR_LEFT_LIGHT_DAMAGE",          0x0070f708},
        {"REAR_RIGHT_LIGHT_DAMAGE",         0x0070f70c},
        {"SPOT_LIGHT_DAMAGE",               0x0070f710},
        {"BRAKE_LIGHT_DAMAGE",              0x0070f714},
        {"STEERING_DAMAGE",                 0x0070f71c},
        {"FRONT_LEFT_SUSPENSION_DAMAGE",    0x0070f720},
        {"FRONT_RIGHT_SUSPENSION_DAMAGE",   0x0070f724},
        {"REAR_LEFT_SUSPENSION_DAMAGE",     0x0070f728},
        {"REAR_RIGHT_SUSPENSION_DAMAGE",    0x0070f72c},
        {"FRONT_BRAKES_DAMAGE",             0x0070f734},
        {"REAR_BRAKES_DAMAGE",              0x0070f738},
        {"FRONT_LEFT_TYRE_DAMAGE",          0x0070f740},
        {"FRONT_RIGHT_TYRE_DAMAGE",         0x0070f744},
        {"REAR_LEFT_TYRE_DAMAGE",           0x0070f748},
        {"REAR_RIGHT_TYRE_DAMAGE",          0x0070f74c}
    };

    // Default values
    inline constexpr float  DEFAULT_GRAVITY                 = 9.8f;
    inline constexpr int    DEFAULT_SUSPENSION_HEIGHT       = 128;
    inline constexpr int    DEFAULT_SUSPENSION_STIFFNESS    = 64;

    // Freeze flag
    inline std::atomic<bool> freezeValue{false};
}