#pragma once
#include <chrono>

#include "Singleton.h"
#include "Settings.h"

namespace dae
{
    class GameTime final : public Singleton<GameTime>
    {
    public:
        void Start();
        void Update();

        float GetDeltaTime() const { return m_SmoothedDeltaTime * m_TimeScale; }
        float GetUnScaledDeltaTime() const { return m_SmoothedDeltaTime; }
        float GetFixedDeltaTime() const { return Settings::fixed_time_step; }

        void SetTimeScale(float timeScale) { m_TimeScale = timeScale; }
        float GetTimeScale() const { return m_TimeScale; }

    private:
        friend class Singleton<GameTime>;
        GameTime() = default;

        std::chrono::high_resolution_clock::time_point m_startTime;
        std::chrono::high_resolution_clock::time_point m_lastTime;
        float m_DeltaTime{};
        float m_TimeScale{ 1.0f };

        // Smoothing variables
        const int m_DeltaTimeWindow{ 15 };
        float m_SmoothedDeltaTime{};
        float m_DeltaTimeBuffer[15]{};
        int m_BufferIndex{};
    };
}
