#pragma once
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <filesystem>
#include <regex>
#include <iostream>
#include "BaseComponent.h"

namespace galaga
{
    class WaveManager : public dae::BaseComponent
    {
    public:
        WaveManager(dae::GameObject* gameObject);
        static void Initialize();

        static void LoadWavesFromFile(const std::string& filename);
        void Update() override;

    private:
        static inline float m_BorderPadding = 40.f;
        static inline float m_TopOffset = 10.f;
        static inline float m_EnemyWidth = 35.f;
        static inline int m_WaveNumber = 0;

        static inline std::vector<std::vector<std::vector<std::pair<std::string, std::pair<int, int>>>>> m_Waves{};
        static void ParseWaveLine(const std::string& line, std::vector<std::pair<std::string, std::pair<int, int>>>& wave);

        // Specific spawn functions for different enemy types
        static void SpawnBee(int x, int y, float moveDistance);
        static void SpawnButterfly(int x, int y, float moveDistance);
        static void SpawnBossGalaga(int x, int y, float moveDistance);

        static float CalculateMovementDistance(int numEnemies);

        // New members for delayed spawning
        static void StartWave(int waveNumber);
        static void SpawnNextEnemy();
        static void ActivateAllEnemies();

        struct EnemySpawnInfo
        {
            std::string type;
            int x;
            int y;
            float moveDistance;
            int order;  // External order
            int subOrder;  // Internal order
        };

        static inline std::queue<EnemySpawnInfo> m_EnemyQueue{};
        static inline std::vector<dae::GameObject*> m_SpawnedEnemies{};
        static inline std::chrono::time_point<std::chrono::steady_clock> m_LastSpawnTime{};
        static inline std::chrono::milliseconds m_SpawnDelay{ 250 };
    };
}