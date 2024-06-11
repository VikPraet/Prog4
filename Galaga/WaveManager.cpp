#include "WaveManager.h"
#include "SceneManager.h"
#include "Settings.h"

#include <fstream>
#include <filesystem>
#include <regex>
#include <iostream>
#include <iso646.h>

#include "ColliderComponent.h"
#include "ColliderRenderComponent.h"
#include "RenderComponent.h"
#include "BasicEnemyMovementBehavior.h"
#include "AnimatorComponent.h"
#include "EnemyCollisionComponent.h"
#include "Health.h"
#include "PathMovement.h"

using namespace std::chrono;

galaga::WaveManager::WaveManager(dae::GameObject* gameObject)
    : BaseComponent(gameObject)
{
    gameObject->SetTag("WaveManager");
    LoadPathsFromFile("paths.txt");
    LoadWavesFromFile("waves.txt");
    StartWave(m_WaveNumber);
}

void galaga::WaveManager::Update()
{
	const auto enemies = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("enemy");
    // If there are no active enemies and no enemies left to spawn, start the next wave
    if (enemies.empty() && m_EnemyQueue.empty())
    {
        StartWave(m_WaveNumber);
    }

    // Check if it's time to spawn the next enemy
    if (!m_EnemyQueue.empty() && duration_cast<milliseconds>(steady_clock::now() - m_LastSpawnTime) > m_SpawnDelay)
    {
        SpawnNextEnemy();
        m_LastSpawnTime = steady_clock::now();
    }

    // check if they can start moving
    bool canStartMoving{true};
    for (const auto& enemy : enemies)
    {
        if (!enemy) continue;
        const auto pathMovement = enemy->GetComponent<PathMovement>();
        if (!pathMovement) continue;
        if (!pathMovement->IsPathComplete())
        {
            canStartMoving = false;
            break;
        }
    }

    if (canStartMoving)
    {
        ActivateAllEnemies();
    }
}

void galaga::WaveManager::StartWave(int waveNumber)
{
    if (waveNumber >= static_cast<int>(m_Waves.size()))
        return;

    m_WaveNumber = waveNumber;
    m_EnemiesToActivate = 0; // Reset the counter for enemies to activate

    const std::vector<std::vector<std::tuple<std::string, int, int, std::string>>>& wave = m_Waves[waveNumber];
    int maxEnemiesInRow = 0;
    for (const auto& row : wave)
    {
        if (static_cast<int>(row.size()) > maxEnemiesInRow)
        {
            maxEnemiesInRow = static_cast<int>(row.size());
        }
    }

    const float moveDistance = CalculateMovementDistance(maxEnemiesInRow);

    for (int row = 0; row < static_cast<int>(wave.size()); ++row)
    {
        const std::vector<std::tuple<std::string, int, int, std::string>>& enemyRow = wave[row];
        const int numEnemies = static_cast<int>(enemyRow.size());
        const float rowWidth = numEnemies * m_EnemyWidth;
        float startX = (dae::Settings::window_width - rowWidth) / 2.0f;

        // Ensure enemies stay at least m_BorderPadding units away from the edges
        if (startX < m_BorderPadding) startX = m_BorderPadding;
        if (startX + rowWidth > dae::Settings::window_width - m_BorderPadding)
            startX = dae::Settings::window_width - m_BorderPadding - rowWidth;

        for (int col = 0; col < static_cast<int>(enemyRow.size()); ++col)
        {
            const auto& [enemyType, order, subOrder, pathName] = enemyRow[col];
            int x = static_cast<int>(startX + col * m_EnemyWidth + m_EnemyWidth / 2);
            int y = static_cast<int>(m_TopOffset) + row * static_cast<int>(m_EnemyWidth);

            // Retrieve the path associated with this enemy type
            std::vector<glm::vec2> path;
            if (!pathName.empty() && pathName != "0")
            {
                auto it = std::ranges::find_if(m_Paths, [&pathName](const Path& p) { return p.name == pathName; });
                if (it != m_Paths.end())
                {
                    path = it->points;
                }
            }

            m_EnemyQueue.push({ enemyType, x, y, moveDistance, order, subOrder, path });
            m_EnemiesToActivate++; // Increment the counter for enemies to activate
        }
    }

    // Sort the queue based on the external order first, then the internal order
    std::vector<EnemySpawnInfo> tempQueue;
    while (!m_EnemyQueue.empty())
    {
        tempQueue.push_back(m_EnemyQueue.front());
        m_EnemyQueue.pop();
    }

    std::ranges::sort(tempQueue, [](const EnemySpawnInfo& a, const EnemySpawnInfo& b)
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

    const EnemySpawnInfo info = m_EnemyQueue.front();
    m_EnemyQueue.pop();

    if (info.type == "Bee")
    {
        SpawnBee(info.x, info.y, info.moveDistance, info.path);
    }
    else if (info.type == "Butterfly")
    {
        SpawnButterfly(info.x, info.y, info.moveDistance, info.path);
    }
    else if (info.type == "BossGalaga")
    {
        SpawnBossGalaga(info.x, info.y, info.moveDistance, info.path);
    }
}

void galaga::WaveManager::ActivateAllEnemies()
{
    for (const auto& enemy : dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("enemy"))
    {
        if (!enemy) continue;

        if (const auto movementComponent = enemy->GetComponent<BasicEnemyMovementBehavior>())
        {
            movementComponent->SetActive(true);
        }
    }
}

void galaga::WaveManager::OnEnemyPathComplete()
{
    for (const auto& enemy : dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("enemy"))
    {
        if (!enemy) continue;

        const auto pathMovement = enemy->GetComponent<PathMovement>();
        if (!pathMovement) continue;
        if (pathMovement->IsPathComplete())
        {
            if (const auto transformComponent = enemy->GetComponent<dae::TransformComponent>())
            {
                transformComponent->SetRotation(0.f);
            }
        }
    }
}

void galaga::WaveManager::LoadPathsFromFile(const std::string& filename)
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
        Path currentPath;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
            {
                if (!currentPath.name.empty())
                {
                    m_Paths.push_back(currentPath);
                    currentPath = Path{};
                }
                continue;
            }

            if (currentPath.name.empty())
            {
                currentPath.name = line;
            }
            else
            {
                float x, y;
                std::istringstream ss(line);
                ss >> x;
                ss.ignore(1, ',');
                ss >> y;
                currentPath.points.emplace_back(x, y);
            }
        }

        if (!currentPath.name.empty())
        {
            m_Paths.push_back(currentPath);
        }
        file.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
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
        std::vector<std::vector<std::tuple<std::string, int, int, std::string>>> currentWave;

        while (std::getline(file, line))
        {
            if (line[0] == '#') continue;
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
                std::vector<std::tuple<std::string, int, int, std::string>> row;
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

void galaga::WaveManager::ParseWaveLine(const std::string& line, std::vector<std::tuple<std::string, int, int, std::string>>& wave)
{
    std::regex regexPattern(R"((\w+)(?:\[(\d+)\])?(?:\{(\d+)\})?(?:\((\w+)\))?(?:\s*\*(\d+))?)");
    std::sregex_iterator iter(line.begin(), line.end(), regexPattern);
    std::sregex_iterator end;

    while (iter != end)
    {
        std::string enemyType = iter->str(1);
        int order = iter->str(2).empty() ? 0 : std::stoi(iter->str(2));
        int subOrder = iter->str(3).empty() ? 0 : std::stoi(iter->str(3));
        std::string pathName = iter->str(4).empty() ? "0" : iter->str(4);
        int count = iter->str(5).empty() ? 1 : std::stoi(iter->str(5));

        for (int i{}; i < count; ++i)
        {
            wave.emplace_back(enemyType, order, subOrder, pathName);
        }
        ++iter;
    }
}

void galaga::WaveManager::SpawnBee(int x, int y, float moveDistance, const std::vector<glm::vec2>& path)
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
    // path follow
    bee->AddComponent<PathMovement>(bee.get(), path, 100.f, true);
    const auto pathFollow = bee->GetComponent<PathMovement>();
    pathFollow->AddWorldSpacePoint({ x, y });
    pathFollow->StartAtFirstPoint();
    pathFollow->OnPathCompleted.AddListener(this, &WaveManager::OnEnemyPathComplete);
    // Health
    bee->AddComponent<Health>(bee.get());
    // Collider
    bee->AddComponent<dae::ColliderComponent>(bee.get(), glm::vec2(28.f, 28.f));
    bee->AddComponent<EnemyCollisionComponent>(bee.get());
    //bee->AddComponent<dae::ColliderRenderComponent>(bee.get());

    // Add the enemy to the scene
    bee->SetTag("enemy");
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bee));
}

void galaga::WaveManager::SpawnButterfly(int x, int y, float moveDistance, const std::vector<glm::vec2>& path)
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
    // path follow
    butterfly->AddComponent<PathMovement>(butterfly.get(), path, 100.f, true);
    const auto pathFollow = butterfly->GetComponent<PathMovement>();
    pathFollow->AddWorldSpacePoint({ x, y });
    pathFollow->StartAtFirstPoint();
    pathFollow->OnPathCompleted.AddListener(this, &WaveManager::OnEnemyPathComplete);
    // Health
    butterfly->AddComponent<Health>(butterfly.get());
    // Collider
    butterfly->AddComponent<dae::ColliderComponent>(butterfly.get(), glm::vec2(30.f, 30.f));
    butterfly->AddComponent<EnemyCollisionComponent>(butterfly.get());
    //butterfly->AddComponent<dae::ColliderRenderComponent>(butterfly.get());

    // Add the enemy to the scene
    butterfly->SetTag("enemy");
    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(butterfly));
}

void galaga::WaveManager::SpawnBossGalaga(int x, int y, float moveDistance, const std::vector<glm::vec2>& path)
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
    // path follow
    bossGalaga->AddComponent<PathMovement>(bossGalaga.get(), path, 100.f, true);
    const auto pathFollow = bossGalaga->GetComponent<PathMovement>();
    pathFollow->AddWorldSpacePoint({ x, y });
    pathFollow->StartAtFirstPoint();
    pathFollow->OnPathCompleted.AddListener(this, &WaveManager::OnEnemyPathComplete);
    // Health
    bossGalaga->AddComponent<Health>(bossGalaga.get(), 2);
    // Collider
    bossGalaga->AddComponent<dae::ColliderComponent>(bossGalaga.get(), glm::vec2(32.f, 32.f));
    bossGalaga->AddComponent<EnemyCollisionComponent>(bossGalaga.get());
    //bossGalaga->AddComponent<dae::ColliderRenderComponent>(bossGalaga.get());

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
