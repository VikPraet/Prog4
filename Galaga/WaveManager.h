#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
#include "Singleton.h"

namespace galaga
{
    class WaveManager final : public dae::Singleton<WaveManager>
    {
    public:
        WaveManager() = default;

        void LoadWavesFromFile(const std::string& filename);
        void SpawnWave(int waveNumber);

    private:
        friend class Singleton<WaveManager>;

        inline static float m_BorderPadding = 40.f;
        inline static float m_TopOffset = 10.f;
        inline static float m_EnemyWidth = 35.f;

        std::vector<std::vector<std::vector<std::string>>> waves;
        void ParseWaveLine(const std::string& line, std::vector<std::string>& wave);

        // Specific spawn functions for different enemy types
        void SpawnBee(int x, int y, float moveDistance);
        void SpawnButterfly(int x, int y, float moveDistance);
        void SpawnBossGalaga(int x, int y, float moveDistance);

        static float CalculateMovementDistance(int numEnemies);
    };
}
