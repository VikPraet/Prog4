#pragma once
#include <chrono>

#include "Singleton.h"

class GameTime final : public dae::Singleton<GameTime>
{
public:
    void Start();
    void Update();

    float GetDeltaTime() const { return m_SmoothedDeltaTime; }
    float GetFixedDeltaTime() const { return m_FixedTimeStep; }

private:
    friend class Singleton<GameTime>;
    GameTime() = default;

    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_lastTime;
    float m_DeltaTime{};
    float m_FixedDeltaTime{};
    const float m_FixedTimeStep{ 0.02f };

    // Smoothing variables
    const int m_DeltaTimeWindow{ 15 };
    float m_SmoothedDeltaTime{};
    float m_DeltaTimeBuffer[15]{};
    int m_BufferIndex{};
};
