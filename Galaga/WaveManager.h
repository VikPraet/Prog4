#pragma once
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <tuple>
#include <glm/vec2.hpp>
#include "BaseComponent.h"
#include <unordered_set>
#include <random>

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
        void OnEnemyAttackComplete(dae::GameObject* enemy);
        void OnEnemyKilled(dae::GameObject* enemy);

    private:
        float m_BorderPadding = 40.f;
        float m_TopOffset = 35.f;
        float m_EnemyWidth = 35.f;
        int m_WaveNumber = 0;
        int m_CurrentGroup = 0;
        int m_EnemiesInCurrentWave = 0;

        struct Path {
            std::string name;
            std::vector<glm::vec2> points;
        };

        std::vector<Path> m_Paths;
        std::vector<std::vector<std::vector<std::tuple<std::string, int, int, std::string, int>>>> m_Waves{};
        void ParseWaveLine(const std::string& line, std::vector<std::tuple<std::string, int, int, std::string, int>>& wave);

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
            bool spawnTogether;
        };

        std::vector<std::queue<EnemySpawnInfo>> m_GroupQueues;
        std::chrono::time_point<std::chrono::steady_clock> m_LastSpawnTime{};
        std::chrono::milliseconds m_SpawnDelay{ 85 };

        void CheckAndStartNextGroup();
        void CheckAndStartNextWave();

        // New members for managing active attackers
        bool m_CanAttack{ false };
        std::unordered_set<dae::GameObject*> m_ActiveAttackers;
        int m_MaxActiveAttackers{ 3 };

        // Random delay range
        std::chrono::milliseconds m_MinAttackDelay{ 150 };
        std::chrono::milliseconds m_MaxAttackDelay{ 500 };
        std::chrono::time_point<std::chrono::steady_clock> m_LastAttackTime{};

        void StartEnemyAttack(const std::vector<dae::GameObject*>& enemies);
        std::vector<dae::GameObject*> SelectRandomAttackers(const std::vector<dae::GameObject*>& enemies) const;
        std::chrono::milliseconds GetRandomAttackDelay() const;
    };
}