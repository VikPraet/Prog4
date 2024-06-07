#include "WaveManager.h"
#include "SceneManager.h"
#include "Settings.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <iostream>

#include "ColliderComponent.h"
#include "ColliderRenderComponent.h"
#include "RenderComponent.h"
#include "BasicEnemyMovementBehavior.h"
#include "AnimatorComponent.h"
#include "EnemyCollisionComponent.h"

void galaga::WaveManager::LoadWavesFromFile(const std::string& filename)
{
    std::filesystem::path filePath = std::filesystem::path("../Data") / filename;

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
        std::vector<std::vector<std::string>> currentWave;

        while (std::getline(file, line))
        {
            if (line.empty())
            {
                if (!currentWave.empty())
                {
                    waves.push_back(currentWave);
                    currentWave.clear();
                }
            }
            else
            {
                std::vector<std::string> row;
                ParseWaveLine(line, row);
                currentWave.push_back(row);
            }
        }

        if (!currentWave.empty())
        {
            waves.push_back(currentWave);
        }

        file.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void galaga::WaveManager::ParseWaveLine(const std::string& line, std::vector<std::string>& wave)
{
    std::regex regexPattern(R"((\w+)(?:\s*x(\d+))?)");
    std::sregex_iterator iter(line.begin(), line.end(), regexPattern);
    std::sregex_iterator end;

    while (iter != end)
    {
        std::string enemyType = iter->str(1);
        int count = iter->str(2).empty() ? 1 : std::stoi(iter->str(2));

        for (int i = 0; i < count; ++i)
        {
            wave.push_back(enemyType);
        }
        ++iter;
    }
}

void galaga::WaveManager::SpawnWave(int waveNumber)
{
    if (waveNumber >= static_cast<int>(waves.size()))
        return;

    const std::vector<std::vector<std::string>>& wave = waves[waveNumber];
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
        const std::vector<std::string>& enemyRow = wave[row];
        int numEnemies = static_cast<int>(enemyRow.size());
        float rowWidth = numEnemies * m_EnemyWidth;
        float startX = (dae::Settings::window_width - rowWidth) / 2.0f;

        // Ensure enemies stay at least m_BorderPadding units away from the edges
        if (startX < m_BorderPadding) startX = m_BorderPadding;
        if (startX + rowWidth > dae::Settings::window_width - m_BorderPadding)
            startX = dae::Settings::window_width - m_BorderPadding - rowWidth;

        for (int col = 0; col < static_cast<int>(enemyRow.size()); ++col)
        {
            const std::string& enemyType = enemyRow[col];
            int x = static_cast<int>(startX + col * m_EnemyWidth + m_EnemyWidth / 2);  // Adjust for centered image
            int y = static_cast<int>(m_TopOffset) + row * static_cast<int>(m_EnemyWidth);

            if (enemyType == "Bee")
            {
                SpawnBee(x, y, moveDistance);
            }
            else if (enemyType == "Butterfly")
            {
                SpawnButterfly(x, y, moveDistance);
            }
            else if (enemyType == "BossGalaga")
            {
                SpawnBossGalaga(x, y, moveDistance);
            }
        }
    }
}

float galaga::WaveManager::CalculateMovementDistance(int numEnemies)
{
    const float screenWidth = static_cast<float>(dae::Settings::window_width);
    const float totalEnemyWidth = numEnemies * m_EnemyWidth;
    const float moveDistance = (screenWidth - totalEnemyWidth - 2 * m_BorderPadding) / 2.0f;
    return moveDistance;
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
    bee->AddComponent<dae::AnimatorComponent>(bee.get(), 1, 2, 1.f);
    // movement
    bee->AddComponent<BasicEnemyMovementBehavior>(bee.get());
    const auto movement = bee->GetComponent<BasicEnemyMovementBehavior>();
    movement->SetBounds(x - moveDistance, x + moveDistance);
    // Collider
    bee->AddComponent<dae::ColliderComponent>(bee.get(), glm::vec2(28.f, 28.f));
    bee->AddComponent<EnemyCollisionComponent>(bee.get());
    //bee->AddComponent<dae::ColliderRenderComponent>(bee.get());

    // Add the enemy to the scene
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
    butterfly->AddComponent<dae::AnimatorComponent>(butterfly.get(), 1, 2, 1.f);
    // movement
    butterfly->AddComponent<BasicEnemyMovementBehavior>(butterfly.get());
    const auto movement = butterfly->GetComponent<BasicEnemyMovementBehavior>();
    movement->SetBounds(x - moveDistance, x + moveDistance);
    // Collider
    butterfly->AddComponent<dae::ColliderComponent>(butterfly.get(), glm::vec2(30.f, 30.f));
    butterfly->AddComponent<EnemyCollisionComponent>(butterfly.get());
    //butterfly->AddComponent<dae::ColliderRenderComponent>(butterfly.get());

    // Add the enemy to the scene
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
    bossGalaga->AddComponent<dae::AnimatorComponent>(bossGalaga.get(), 1, 2, 1.f);
    // movement
    bossGalaga->AddComponent<BasicEnemyMovementBehavior>(bossGalaga.get());
    const auto movement = bossGalaga->GetComponent<BasicEnemyMovementBehavior>();
    movement->SetBounds(x - moveDistance, x + moveDistance);
    // Collider
    bossGalaga->AddComponent<dae::ColliderComponent>(bossGalaga.get(), glm::vec2(32.f, 32.f));
    bossGalaga->AddComponent<EnemyCollisionComponent>(bossGalaga.get());
    //bossGalaga->AddComponent<dae::ColliderRenderComponent>(bossGalaga.get());

    // Add the enemy to the scene
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bossGalaga));
}
