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
    inline constexpr uintptr_t ADDR_LIGHTNING_SOUND_FLAG                = 0x005165a8;
    inline constexpr uintptr_t ADDR_TRIGGER_CHECKPOINT_SOUND            = 0x0051659c;
    inline constexpr uintptr_t ADDR_TRIGGER_LITTLE_TIME_REMAINING_SOUND = 0x005165a0;
    inline constexpr uintptr_t ADDR_TRIGGER_OUT_OF_TIME_SOUND           = 0x005165a4;
    inline constexpr uintptr_t ADDR_TIME_STOPPED_CAR_TO_LOSS            = 0x0070f490;
    inline constexpr uintptr_t ADDR_TIME_STOPPED_CAR_TO_WIN             = 0x0070f494;
    inline constexpr uintptr_t ADDR_GAME_OVER_FLAG                      = 0x0070f49c;
    inline constexpr uintptr_t ADDR_MAX_RESET_SPEED                     = 0x00613828; // in m/s
    inline constexpr uintptr_t ADDR_TRIGGER_RESTART_STAGE               = 0x0070f408;
    inline constexpr uintptr_t ADDR_CAR_ID                              = 0x0070f410;

    inline constexpr uintptr_t ADDR_MAX_DRAW_DISTANCE                   = 0x0071c6d4;
    inline constexpr uintptr_t ADDR_REFLECTIONS                         = 0x006238ac;

    inline constexpr uintptr_t ADDR_BRAKE_BALANCE                       = 0x0070f40c;
    inline constexpr uintptr_t ADDR_GEAR_RATIO                          = 0x0070f414;
    inline constexpr uintptr_t ADDR_TYRE_TYPE                           = 0x0070f418;
    inline constexpr uintptr_t ADDR_SUSPENSION_HEIGHT                   = 0x0070f41c;
    inline constexpr uintptr_t ADDR_SUSPENSION_STIFFNESS                = 0x0070f420;
    inline constexpr uintptr_t ADDR_TRANSMISSION_TYPE                   = 0x0070f424;
    inline constexpr uintptr_t ADDR_STEERING_SENSITIVITY                = 0x0070f428;

    // Idle car part damage over time (RPM/gear based)
    inline constexpr uintptr_t ADDR_TURBO_LOSS_POWER_MULTIPLIER         = 0x0075b5a0;
    inline constexpr uintptr_t ADDR_TURBO_DAMAGE_PER_MINUTE             = 0x0075b5a4;
    inline constexpr uintptr_t ADDR_GEAR_DAMAGE_PER_MINUTE              = 0x0075b5a8;
    inline constexpr uintptr_t ADDR_ENGINE_DAMAGE_PER_MINUTE            = 0x0075b5d0;
    inline constexpr uintptr_t ADDR_EXHAUST_DAMAGE_PER_MINUTE           = 0x0075b5a4;
    inline constexpr uintptr_t ADDR_COOLING_DAMAGE_PER_MINUTE           = 0x0075b5a8;
    inline constexpr uintptr_t ADDR_CLUTCH_DAMAGE_PER_MINUTE            = 0x0075b5d0;
    inline constexpr uintptr_t ADDR_DIFFERENTIAL_DAMAGE_PER_MINUTE      = 0x0075b5a4;
    inline constexpr uintptr_t ADDR_ELECTRICS_DAMAGE_PER_MINUTE         = 0x0075b5a8;
    inline constexpr uintptr_t ADDR_LIGHT_DAMAGE_PER_MINUTE             = 0x0075b5d0;
    inline constexpr uintptr_t ADDR_STEERING_DAMAGE_PER_MINUTE          = 0x0075b5a4;
    inline constexpr uintptr_t ADDR_SUSPENSION_DAMAGE_PER_MINUTE        = 0x0075b5a8;
    inline constexpr uintptr_t ADDR_BRAKE_DAMAGE_PER_MINUTE             = 0x0075b5d0;

    inline constexpr uintptr_t ADDR_CAR_PARTS_LEFT                      = 0x0070f684;

    // Damage addresses
    inline std::vector<std::pair<std::string, uintptr_t>> ADDR_DAMAGE = {
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
    inline constexpr float  DEFAULT_MAX_RESET_SPEED         = 5.0f;

    inline constexpr int    DEFAULT_BRAKE_BALANCE           = 128;
    inline constexpr int    DEFAULT_GEAR_RATIO              = 0;
    inline constexpr int    DEFAULT_TYRE_TYPE               = 0;
    inline constexpr int    DEFAULT_SUSPENSION_HEIGHT       = 128;
    inline constexpr int    DEFAULT_SUSPENSION_STIFFNESS    = 64;
    inline constexpr int    DEFAULT_TRANSMISSION_TYPE       = 0;
    inline constexpr int    DEFAULT_STEERING_SENSITIVITY    = 0;

    inline constexpr int    DEFAULT_MAX_DRAW_DISTANCE       = 12;
    inline constexpr int    DEFAULT_REFLECTIONS             = 1;

    // Freeze flag
    inline std::atomic<bool> freezeValue{false};

    // Int-coded variable lists w/ corresponding internal IDs
    inline std::vector<std::pair<int, std::string>> CARS = {
        {0, "Renault Maxi Megane"},
        {1, "Vauxhall Astra"},
        {2, "Seat Cordoba"},
        {3, "Hyundai Coupe"},
        {4, "Subaru Impreza WRC"},
        {5, "Peugeot 106"},
        {6, "Citroen Saxo WRC"},
        {7, "Mitsubishi Lancer Evo V"},
        {8, "Volkswagen Golf MkIV"},
        {9, "Nissan Almera"},
        {10, "Proton Wira/Persona"},
        {11, "Citroen Saxo"},
        {12, "Honda Civic"},
        {13, "Nissan Micra"},
        {14, "Peuegot 206 WRC"},
        {15, "Ford Escort RS2000"},
        {16, "Skoda Octavia"},
        {17, "Seat Ibiza"},
        {18, "Skoda Felicia"},
        {19, "Proton Compact"},
        {20, "Ford Escort Maxi"},
        {21, "Ford Puma"},
        {22, "Mitsubishi Lancer Evo IV"},
        {23, "MF Turbo"},
        {24, "MF Hothatch"},
        {25, "Bernie's Dicemobile"},
        {26, "Skip"},
        {27, "Turbotater"},
        {28, "Moo 1.8BSE Turbo"},
        {29, "Lambaaghini"},
        {30, "Radio Car"},
        {31, "Welsh Sheep Wagon"},
        {32, "Local Shop"},
        {33, "Renault Sport Clio"}
    };
    inline std::vector<std::pair<int, std::string>> GEAR_RATIOS = {
        {0, "Shortest"},
        {1, "Short"},
        {2, "Medium"},
        {3, "Long"},
        {4, "Longest"}
    };
    inline std::vector<std::pair<int, std::string>> TYRE_TYPES = {
        {0, "Slicks"},
        {1, "Intermediate"},
        {2, "Wets"},
        {3, "Dry gravel"},
        {4, "Wet gravel"},
        {5, "Snow"}
    };
    inline std::vector<std::pair<int, std::string>> TRANSMISSION_TYPES = {
        {0, "Manual"},
        {1, "Semi-auto"},
        {2, "Automatic"}
    };
}