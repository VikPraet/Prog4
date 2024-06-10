#include "WaveManager.h"
#include "SceneManager.h"
#include "Settings.h"

#include <fstream>
#include <filesystem>
#include <regex>
#include <iostream>

#include "ColliderComponent.h"
#include "ColliderRenderComponent.h"
#include "RenderComponent.h"
#include "BasicEnemyMovementBehavior.h"
#include "AnimatorComponent.h"
#include "EnemyCollisionComponent.h"

using namespace std::chrono;

galaga::WaveManager::WaveManager(dae::GameObject* gameObject)
    : BaseComponent(gameObject)
{
}

void galaga::WaveManager::Initialize()
{
    // Create a new GameObject for WaveManager
    auto waveManagerObject = std::make_unique<dae::GameObject>();
    waveManagerObject->AddComponent<WaveManager>(waveManagerObject.get());
    waveManagerObject->SetTag("WaveManager");
    // Add the GameObject to the active scene
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(waveManagerObject));

    LoadWavesFromFile("waves.txt");
    StartWave(m_WaveNumber);
}

void galaga::WaveManager::Update()
{
    // If there are no active enemies and no enemies left to spawn, start the next wave
    if (dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("enemy").empty() && m_EnemyQueue.empty())
    {
        StartWave(m_WaveNumber);
    }

    // Check if it's time to spawn the next enemy
    if (!m_EnemyQueue.empty() && duration_cast<milliseconds>(steady_clock::now() - m_LastSpawnTime) > m_SpawnDelay)
    {
        SpawnNextEnemy();
        m_LastSpawnTime = steady_clock::now();
    }

    // Activate all enemies if they are all spawned
    if (m_EnemyQueue.empty() && !m_SpawnedEnemies.empty())
    {
        ActivateAllEnemies();
        m_SpawnedEnemies.clear();
    }
}

void galaga::WaveManager::StartWave(int waveNumber)
{
    if (waveNumber >= static_cast<int>(m_Waves.size()))
        return;

    m_WaveNumber = waveNumber;

    const std::vector<std::vector<std::pair<std::string, std::pair<int, int>>>>& wave = m_Waves[waveNumber];
    int maxEnemiesInRow = 0;
    for (const auto& row : wave)
    {
        if (static_cast<int>(row.size()) > maxEnemiesInRow)
        {
            maxEnemiesInRow = static_cast<int>(row.size());
        }
    }

    float moveDistance = CalculateMovementDistance(maxEnemiesInRow);

    for (int row = 0; row < static_cast<int>(wave.size()); ++row)
    {
        const std::vector<std::pair<std::string, std::pair<int, int>>>& enemyRow = wave[row];
        int numEnemies = static_cast<int>(enemyRow.size());
        float rowWidth = numEnemies * m_EnemyWidth;
        float startX = (dae::Settings::window_width - rowWidth) / 2.0f;

        // Ensure enemies stay at least m_BorderPadding units away from the edges
        if (startX < m_BorderPadding) startX = m_BorderPadding;
        if (startX + rowWidth > dae::Settings::window_width - m_BorderPadding)
            startX = dae::Settings::window_width - m_BorderPadding - rowWidth;

        for (int col = 0; col < static_cast<int>(enemyRow.size()); ++col)
        {
            const auto& [enemyType, orders] = enemyRow[col];
            int x = static_cast<int>(startX + col * m_EnemyWidth + m_EnemyWidth / 2);  // Adjust for centered image
            int y = static_cast<int>(m_TopOffset) + row * static_cast<int>(m_EnemyWidth);

            m_EnemyQueue.push({ enemyType, x, y, moveDistance, orders.first, orders.second });
        }
    }

    // Sort the queue based on the external order first, then the internal order
    std::vector<EnemySpawnInfo> tempQueue;
    while (!m_EnemyQueue.empty())
    {
        tempQueue.push_back(m_EnemyQueue.front());
        m_EnemyQueue.pop();
    }

    std::sort(tempQueue.begin(), tempQueue.end(), [](const EnemySpawnInfo& a, const EnemySpawnInfo& b)
        {
            if (a.order == b.order)
                return a.subOrder < b.subOrder;
            return a.order < b.order;
        });

    for (const auto& info : tempQueue)
    {
        m_EnemyQueue.push(info);
    }

    m_WaveNumber++;
    m_LastSpawnTime = steady_clock::now();
}

void galaga::WaveManager::SpawnNextEnemy()
{
    if (m_EnemyQueue.empty())
        return;

    EnemySpawnInfo info = m_EnemyQueue.front();
    m_EnemyQueue.pop();

    if (info.type == "Bee")
    {
        SpawnBee(info.x, info.y, info.moveDistance);
    }
    else if (info.type == "Butterfly")
    {
        SpawnButterfly(info.x, info.y, info.moveDistance);
    }
    else if (info.type == "BossGalaga")
    {
        SpawnBossGalaga(info.x, info.y, info.moveDistance);
    }
}

void galaga::WaveManager::ActivateAllEnemies()
{
    for (const auto& enemy : m_SpawnedEnemies)
    {
        if (!enemy) continue;

        const auto movementComponent = enemy->GetComponent<BasicEnemyMovementBehavior>();
        if (movementComponent)
        {
            movementComponent->SetActive(true);
        }
    }
}

void galaga::WaveManager::LoadWavesFromFile(const std::string& filename)
{
    const std::filesystem::path filePath = std::filesystem::path("../Data/") / filename;

    try
    {
        if (!exists(filePath))
        {
            throw std::runtime_error("File does not exist: " + filePath.string());
        }

        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Unable to open file: " + filePath.string());
        }

        std::string line;
        std::vector<std::vector<std::pair<std::string, std::pair<int, int>>>> currentWave;

        while (std::getline(file, line))
        {
            if (line.empty())
            {
                if (!currentWave.empty())
                {
                    m_Waves.push_back(currentWave);
                    currentWave.clear();
                }
            }
            else
            {
                std::vector<std::pair<std::string, std::pair<int, int>>> row;
                ParseWaveLine(line, row);
                currentWave.push_back(row);
            }
        }

        if (!currentWave.empty())
        {
            m_Waves.push_back(currentWave);
        }

        file.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void galaga::WaveManager::ParseWaveLine(const std::string& line, std::vector<std::pair<std::string, std::pair<int, int>>>& wave)
{
    std::regex regexPattern(R"((\w+)(?:\[(\d+)\])?(?:\{(\d+)\})?(?:\s*\*(\d+))?)");
    std::sregex_iterator iter(line.begin(), line.end(), regexPattern);
    std::sregex_iterator end;

    while (iter != end)
    {
        std::string enemyType = iter->str(1);
        int order = iter->str(2).empty() ? 0 : std::stoi(iter->str(2));
        int subOrder = iter->str(3).empty() ? 0 : std::stoi(iter->str(3));
        int count = iter->str(4).empty() ? 1 : std::stoi(iter->str(4));

        for (int i = 0; i < count; ++i)
        {
            wave.emplace_back(enemyType, std::make_pair(order, subOrder));
        }
        ++iter;
    }
}

void galaga::WaveManager::SpawnBee(int x, int y, float moveDistance)
{
    auto bee = std::make_unique<dae::GameObject>();
    // transform
    bee->AddComponent<dae::TransformComponent>(bee.get());
    bee->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(x), static_cast<float>(y));
    bee->GetComponent<dae::TransformComponent>()->SetScale(2);
    // render
    bee->AddComponent<dae::RenderComponent>(bee.get());
    bee->GetComponent<dae::RenderComponent>()->SetTexture("galaga-bee-idle.png");
    // animator
    bee->AddComponent<dae::AnimatorComponent>(bee.get(), 1, 2, 1.f, true);
    // movement
    bee->AddComponent<BasicEnemyMovementBehavior>(bee.get(), 35.f);
    const auto movement = bee->GetComponent<BasicEnemyMovementBehavior>();
    movement->SetBounds(x - moveDistance, x + moveDistance);
    movement->SetActive(false); // Set movement component inactive
    // Collider
    bee->AddComponent<dae::ColliderComponent>(bee.get(), glm::vec2(28.f, 28.f));
    bee->AddComponent<EnemyCollisionComponent>(bee.get());
    //bee->AddComponent<dae::ColliderRenderComponent>(bee.get());

    m_SpawnedEnemies.push_back(bee.get());
    // Add the enemy to the scene
    bee->SetTag("enemy");
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bee));
}

void galaga::WaveManager::SpawnButterfly(int x, int y, float moveDistance)
{
    auto butterfly = std::make_unique<dae::GameObject>();
    // transform
    butterfly->AddComponent<dae::TransformComponent>(butterfly.get());
    butterfly->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(x), static_cast<float>(y));
    butterfly->GetComponent<dae::TransformComponent>()->SetScale(2);
    // render
    butterfly->AddComponent<dae::RenderComponent>(butterfly.get());
    butterfly->GetComponent<dae::RenderComponent>()->SetTexture("galaga-butterfly-idle.png");
    // animator
    butterfly->AddComponent<dae::AnimatorComponent>(butterfly.get(), 1, 2, 1.f, true);
    // movement
    butterfly->AddComponent<BasicEnemyMovementBehavior>(butterfly.get(), 35.f);
    const auto movement = butterfly->GetComponent<BasicEnemyMovementBehavior>();
    movement->SetBounds(x - moveDistance, x + moveDistance);
    movement->SetActive(false); // Set movement component inactive
    // Collider
    butterfly->AddComponent<dae::ColliderComponent>(butterfly.get(), glm::vec2(30.f, 30.f));
    butterfly->AddComponent<EnemyCollisionComponent>(butterfly.get());
    //butterfly->AddComponent<dae::ColliderRenderComponent>(butterfly.get());

    m_SpawnedEnemies.push_back(butterfly.get());
    // Add the enemy to the scene
    butterfly->SetTag("enemy");
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(butterfly));
}

void galaga::WaveManager::SpawnBossGalaga(int x, int y, float moveDistance)
{
    auto bossGalaga = std::make_unique<dae::GameObject>();
    // transform
    bossGalaga->AddComponent<dae::TransformComponent>(bossGalaga.get());
    bossGalaga->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(x), static_cast<float>(y));
    bossGalaga->GetComponent<dae::TransformComponent>()->SetScale(2);
    // render
    bossGalaga->AddComponent<dae::RenderComponent>(bossGalaga.get());
    bossGalaga->GetComponent<dae::RenderComponent>()->SetTexture("galaga-boss-idle.png");
    // animator
    bossGalaga->AddComponent<dae::AnimatorComponent>(bossGalaga.get(), 1, 2, 1.f, true);
    // movement
    bossGalaga->AddComponent<BasicEnemyMovementBehavior>(bossGalaga.get(), 35.f);
    const auto movement = bossGalaga->GetComponent<BasicEnemyMovementBehavior>();
    movement->SetBounds(x - moveDistance, x + moveDistance);
    movement->SetActive(false); // Set movement component inactive
    // Collider
    bossGalaga->AddComponent<dae::ColliderComponent>(bossGalaga.get(), glm::vec2(32.f, 32.f));
    bossGalaga->AddComponent<EnemyCollisionComponent>(bossGalaga.get());
    //bossGalaga->AddComponent<dae::ColliderRenderComponent>(bossGalaga.get());

    m_SpawnedEnemies.push_back(bossGalaga.get());
    // Add the enemy to the scene
    bossGalaga->SetTag("enemy");
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bossGalaga));
}

float galaga::WaveManager::CalculateMovementDistance(int numEnemies)
{
    const float screenWidth = static_cast<float>(dae::Settings::window_width);
    const float totalEnemyWidth = numEnemies * m_EnemyWidth;
    const float moveDistance = (screenWidth - totalEnemyWidth - 2 * m_BorderPadding) / 2.0f;
    return moveDistance;
}