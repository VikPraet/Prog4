#pragma once
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <filesystem>
#include <regex>
#include <iostream>
#include <tuple>
#include <glm/vec2.hpp>
#include "BaseComponent.h"

namespace galaga
{
    class WaveManager : public dae::BaseComponent
    {
    public:
        WaveManager(dae::GameObject* gameObject);

        void LoadPathsFromFile(const std::string& filename);
        void LoadWavesFromFile(const std::string& filename);
        void Update() override;

        void OnEnemyPathComplete();

    private:
        float m_BorderPadding = 40.f;
        float m_TopOffset = 35.f;
        float m_EnemyWidth = 35.f;
        int m_WaveNumber = 0;
        int m_CurrentGroup = 0; // Track the current group
        int m_EnemiesInCurrentWave = 0;

        struct Path {
            std::string name;
            std::vector<glm::vec2> points;
        };

        std::vector<Path> m_Paths;
        std::vector<std::vector<std::vector<std::tuple<std::string, int, int, std::string>>>> m_Waves{};
        void ParseWaveLine(const std::string& line, std::vector<std::tuple<std::string, int, int, std::string>>& wave);

        // Specific spawn functions for different enemy types
        void SpawnBee(int x, int y, float moveDistance, const std::vector<glm::vec2>& path);
        void SpawnButterfly(int x, int y, float moveDistance, const std::vector<glm::vec2>& path);
        void SpawnBossGalaga(int x, int y, float moveDistance, const std::vector<glm::vec2>& path);

        float CalculateMovementDistance(int numEnemies);

        // New members for delayed spawning
        void StartWave(int waveNumber);
        void SpawnNextEnemy();
        void ActivateAllEnemies();

        struct EnemySpawnInfo
        {
            std::string type;
            int x;
            int y;
            float moveDistance;
            int order;
            int subOrder;
            std::vector<glm::vec2> path;
        };

        std::vector<std::queue<EnemySpawnInfo>> m_GroupQueues; // Queues for each group
        std::chrono::time_point<std::chrono::steady_clock> m_LastSpawnTime{};
        std::chrono::milliseconds m_SpawnDelay{ 120 };

        void CheckAndStartNextGroup();
        void CheckAndStartNextWave();
    };
}