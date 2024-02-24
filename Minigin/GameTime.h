#pragma once
#include <chrono>

#include "Singleton.h"

class GameTime final : public dae::Singleton<GameTime>
{
public:
    void Start();
    void Update();

    float GetFixedTimeStep() const { return 1/m_FixedTimeStep; }
    float GetDeltaTime() const { return 1 / m_SmoothedDeltaTime; }
    float GetFixedDeltaTime() const { return 1/m_FixedTimeStep; }

private:
    friend class Singleton<GameTime>;
    GameTime() = default;

    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_lastTime;
    float m_DeltaTime{};
    float m_FixedDeltaTime{};
    const float m_FixedTimeStep{ 0.02f };

    // Smoothing variables
    const int m_DeltaTimeWindow{ 25 };
    float m_SmoothedDeltaTime{};
    float m_DeltaTimeBuffer[25]{};
    int m_BufferIndex{};
};
