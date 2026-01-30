#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>

#include "game_memory.h"

template<typename T>
class FreezeManager {
    public:
        FreezeManager();
        ~FreezeManager();

        void AddValue(uintptr_t address, T initialValue);
        void RemoveValue(uintptr_t address);
        void SetValue(uintptr_t address, T value, bool freeze);

    private:
        struct FrozenValue {
            uintptr_t address;
            T value;
            bool active = true;
        };

        void Loop();

        std::vector<FrozenValue> values;
        std::thread loopThread;
        std::atomic<bool> running{true};
        std::mutex valuesMutex; // protects access to values between threads
};

/* ========== IMPLEMENTATION ========== */

template <typename T>
FreezeManager<T>::FreezeManager() {
    loopThread = std::thread(&FreezeManager::Loop, this);
}

template <typename T>
FreezeManager<T>::~FreezeManager() {
    // Stop the loop thread by joining into main thread
    running = false;
    if(loopThread.joinable()) loopThread.join();
}

template <typename T>
void FreezeManager<T>::AddValue(uintptr_t address, T initialValue) {
    std::lock_guard<std::mutex> lock(valuesMutex);
    values.push_back({ address, initialValue, true });
}

template <typename T>
void FreezeManager<T>::RemoveValue(uintptr_t address) {
    std::lock_guard<std::mutex> lock(valuesMutex);
    values.erase(
        std::remove_if(values.begin(), values.end(), [=](auto& v){ return v.address == address; }),
        values.end()
    );
}

template <typename T>
void FreezeManager<T>::SetValue(uintptr_t address, T value, bool freeze) {
    std::lock_guard<std::mutex> lock(valuesMutex);
    for(auto& v : values) {
        if(v.address == address) {
            v.value = value;
            v.active = freeze;
            return;
        }
    }
    // optionally add if not found
    AddValue(address, value);
    values.back().active = freeze;
}

template <typename T>
void FreezeManager<T>::Loop() {
    while(running) {
        // Create a snapshot of current values to prevent iterator invalidation
        std::vector<FrozenValue> snapshot;
        {
            std::lock_guard<std::mutex> lock(valuesMutex);
            snapshot = values; // copy vector safely
        }
        for(auto& v : snapshot) {
            if(v.active) {
                gameMemory::mem<T>(v.address) = v.value;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // approx. 60 Hz
    }
}