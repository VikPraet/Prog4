#pragma once
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <tuple>
#include <glm/vec2.hpp>
#include "BaseComponent.h"
#include <unordered_set>

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

        int GetWaveNumber() const { return m_WaveNumber; }
        void CheckAndStartNextWave();

    private:
        float m_BorderPadding = 40.f;
        float m_TopOffset = 70.f;
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

        void SpawnBee(int x, int y, float moveDistance, const std::vector<glm::vec2>& path);
        void SpawnButterfly(int x, int y, float moveDistance, const std::vector<glm::vec2>& path);
        void SpawnBossGalaga(int x, int y, float moveDistance, const std::vector<glm::vec2>& path);

        float CalculateMovementDistance(int numEnemies);

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
        bool m_ActivatedEnemies{ false };

        void CheckAndStartNextGroup();

        bool m_CanAttack{ false };
        std::unordered_set<dae::GameObject*> m_ActiveAttackers;
        int m_NumAttackersAtOnce{ 3 };

        void StartEnemyAttack();
        std::vector<dae::GameObject*> SelectAttackers(const std::vector<dae::GameObject*>& enemies) const;
    };
}