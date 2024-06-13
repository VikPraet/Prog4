#include "WaveManager.h"
#include "SceneManager.h"
#include "Settings.h"

#include <fstream>
#include <filesystem>
#include <regex>
#include <iostream>
#include <map>
#include <random>

#include "ColliderComponent.h"
#include "ColliderRenderComponent.h"
#include "RenderComponent.h"
#include "BasicEnemyMovementBehavior.h"
#include "AnimatorComponent.h"
#include "BeeAttackBehavior.h"
#include "BossGalaga.h"
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

    // Check if there are no active enemies and all groups in m_GroupQueues are exhausted
    bool allGroupsExhausted = true;
    for (const auto& groupQueue : m_GroupQueues)
    {
        if (!groupQueue.empty())
        {
            allGroupsExhausted = false;
            break;
        }
    }

    if (enemies.empty() && allGroupsExhausted)
    {
        CheckAndStartNextWave();
    }

    // Existing logic for checking if it's time to spawn the next enemy in the current group
    if (!m_GroupQueues.empty() && m_CurrentGroup < static_cast<int>(m_GroupQueues.size()) && !m_GroupQueues[m_CurrentGroup].empty() && duration_cast<milliseconds>(steady_clock::now() - m_LastSpawnTime) > m_SpawnDelay)
    {
        SpawnNextEnemy();
        m_LastSpawnTime = steady_clock::now();
    }

    // Existing logic for checking if all enemies in the current group have completed their paths
    CheckAndStartNextGroup();

    // Manage active attackers
    if (m_CanAttack && m_ActiveAttackers.size() < static_cast<size_t>(m_MaxActiveAttackers))
    {
        if (steady_clock::now() - m_LastAttackTime >= GetRandomAttackDelay())
        {
            const std::vector<dae::GameObject*> enemiesToAttack = SelectRandomAttackers(enemies);
            StartEnemyAttack(enemiesToAttack);
            m_LastAttackTime = steady_clock::now();
        }
    }
}

void galaga::WaveManager::StartWave(int waveNumber)
{
    if (waveNumber >= static_cast<int>(m_Waves.size()))
        return;

    m_CanAttack = false;
    m_WaveNumber = waveNumber;
    m_EnemiesInCurrentWave = 0; // Reset the counter for enemies to activate
    m_CurrentGroup = 0; // Reset the current group
    m_GroupQueues.clear(); // Clear previous group queues

    const std::vector<std::vector<std::tuple<std::string, int, int, std::string, int>>>& wave = m_Waves[waveNumber];
    int maxEnemiesInRow = 0;
    for (const auto& row : wave)
    {
        if (static_cast<int>(row.size()) > maxEnemiesInRow)
        {
            maxEnemiesInRow = static_cast<int>(row.size());
        }
    }

    const float moveDistance = CalculateMovementDistance(maxEnemiesInRow);

    // Map to hold each group based on external order
    std::map<int, std::vector<EnemySpawnInfo>> groups;

    for (int row = 0; row < static_cast<int>(wave.size()); ++row)
    {
        const std::vector<std::tuple<std::string, int, int, std::string, int>>& enemyRow = wave[row];
        const int numEnemies = static_cast<int>(enemyRow.size());
        const float rowWidth = numEnemies * m_EnemyWidth;
        float startX = (dae::Settings::window_width - rowWidth) / 2.0f;

        // Ensure enemies stay at least m_BorderPadding units away from the edges
        if (startX < m_BorderPadding) startX = m_BorderPadding;
        if (startX + rowWidth > dae::Settings::window_width - m_BorderPadding)
            startX = dae::Settings::window_width - m_BorderPadding - rowWidth;

        for (int col = 0; col < static_cast<int>(enemyRow.size()); ++col)
        {
            const auto& [enemyType, order, subOrder, pathName, count] = enemyRow[col];
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

            // Collect enemies in groups by external order
            for (int i = 0; i < count; ++i)
            {
                const bool spawnTogether = count == 1;
                groups[order].emplace_back(EnemySpawnInfo{ enemyType, x, y, moveDistance, order, subOrder, path, spawnTogether });
                m_EnemiesInCurrentWave++; // Increment the counter for enemies in the current wave
            }
        }
    }

    // Sort each group by internal order and push to the group queues
    for (auto& [order, enemies] : groups)
    {
        std::ranges::sort(enemies, [](const EnemySpawnInfo& a, const EnemySpawnInfo& b)
            {
                return a.subOrder < b.subOrder;
            });

        std::queue<EnemySpawnInfo> queue;
        for (const auto& enemy : enemies)
        {
            queue.push(enemy);
        }
        m_GroupQueues.push_back(queue);
    }

    // Start the first group
    m_LastSpawnTime = steady_clock::now();
}

void galaga::WaveManager::SpawnNextEnemy()
{
    if (m_CurrentGroup < static_cast<int>(m_GroupQueues.size()) && m_GroupQueues[m_CurrentGroup].empty())
        return;

    if (!m_GroupQueues[m_CurrentGroup].empty())
    {
        // Get the subOrder of the first enemy in the current group
        int currentSubOrder = m_GroupQueues[m_CurrentGroup].front().subOrder;

        // Collect all enemies with the same subOrder that need to spawn together
        std::vector<EnemySpawnInfo> enemiesToSpawn;
        while (!m_GroupQueues[m_CurrentGroup].empty() && m_GroupQueues[m_CurrentGroup].front().subOrder == currentSubOrder)
        {
            auto enemyInfo = m_GroupQueues[m_CurrentGroup].front();
            m_GroupQueues[m_CurrentGroup].pop();

            if (enemyInfo.spawnTogether || enemiesToSpawn.empty())
            {
                enemiesToSpawn.push_back(enemyInfo);
            }
            else
            {
                // Spawn previously collected enemies first if current enemy does not spawn together
                break;
            }
        }

        // Spawn the collected enemies
        for (const auto& info : enemiesToSpawn)
        {
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

        // If enemies are marked to spawn together, delay the next spawn
        if (!enemiesToSpawn.empty() && !enemiesToSpawn.front().spawnTogether)
        {
            m_LastSpawnTime = steady_clock::now();
        }
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
        if (const auto attackComponent = enemy->GetComponent<EnemyAttackBehavior>())
        {
            attackComponent->SetActive(true);
        }
    }
    m_CanAttack = true;
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

void galaga::WaveManager::StartEnemyAttack(const std::vector<dae::GameObject*>& enemies)
{
    for (auto enemy : enemies)
    {
        if (m_ActiveAttackers.size() >= static_cast<size_t>(m_MaxActiveAttackers))
            break;

        if (const auto attackComponent = enemy->GetComponent<EnemyAttackBehavior>())
        {
            attackComponent->StartAttacking();
            m_ActiveAttackers.insert(enemy);
        }
    }
}

std::vector<dae::GameObject*> galaga::WaveManager::SelectRandomAttackers(const std::vector<dae::GameObject*>& enemies) const
{
    std::vector<dae::GameObject*> availableAttackers;
    for (const auto& enemy : enemies)
    {
        if (!m_ActiveAttackers.contains(enemy))
        {
            availableAttackers.push_back(enemy);
        }
    }

    // Determine the number of attackers to start
    int numAttackers = std::min(static_cast<int>(availableAttackers.size()), m_MaxActiveAttackers - static_cast<int>(m_ActiveAttackers.size()));

    // Shuffle the available attackers
    std::random_device rd;
    std::mt19937 g(rd());
    std::ranges::shuffle(availableAttackers, g);

    // Select the number of attackers to start
    std::vector<dae::GameObject*> selectedAttackers;
    selectedAttackers.reserve(numAttackers);
    for (int i = 0; i < numAttackers; ++i)
    {
        selectedAttackers.push_back(availableAttackers[i]);
    }

    return selectedAttackers;
}

std::chrono::milliseconds galaga::WaveManager::GetRandomAttackDelay() const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(static_cast<int>(m_MinAttackDelay.count()), static_cast<int>(m_MaxAttackDelay.count()));
    return std::chrono::milliseconds(dis(gen));
}

void galaga::WaveManager::OnEnemyAttackComplete(dae::GameObject* enemy)
{
    m_ActiveAttackers.erase(enemy);
}

void galaga::WaveManager::OnEnemyKilled(dae::GameObject* enemy)
{
    m_ActiveAttackers.erase(enemy);
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
        std::vector<std::vector<std::tuple<std::string, int, int, std::string, int>>> currentWave;

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
                std::vector<std::tuple<std::string, int, int, std::string, int>> row;
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

void galaga::WaveManager::ParseWaveLine(const std::string& line, std::vector<std::tuple<std::string, int, int, std::string, int>>& wave)
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
            bool spawnTogether = count == 1;
            wave.emplace_back(enemyType, order, subOrder, pathName, spawnTogether ? 1 : count);
        }
        ++iter;
    }
}

void galaga::WaveManager::CheckAndStartNextGroup()
{
    const auto enemies = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("enemy");
    bool allPathsCompleted = true;
    for (const auto& enemy : enemies)
    {
        if (!enemy) continue;
        const auto pathMovement = enemy->GetComponent<PathMovement>();
        if (!pathMovement) continue;
        if (!pathMovement->IsPathComplete())
        {
            allPathsCompleted = false;
            break;
        }
    }

    if (allPathsCompleted && m_CurrentGroup < static_cast<int>(m_GroupQueues.size()) && m_GroupQueues[m_CurrentGroup].empty())
    {
        m_CurrentGroup++;
        // Check if there are more groups to process
        if (m_CurrentGroup < static_cast<int>(m_GroupQueues.size()))
        {
            m_LastSpawnTime = steady_clock::now(); // Reset the spawn timer for the next group
        }
        else
        {
            // No more groups in the current wave, proceed to activate all enemies in this wave
            ActivateAllEnemies();
        }
    }
}

void galaga::WaveManager::CheckAndStartNextWave()
{
    m_CurrentGroup = 0;
    m_WaveNumber++;
    if (m_WaveNumber >= static_cast<int>(m_Waves.size()))
    {
        m_WaveNumber = 0; // Loop back to the first wave
    }
    StartWave(m_WaveNumber);
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
    movement->ResetPosition({ static_cast<float>(x), static_cast<float>(y) });
    movement->SetActive(false); // Set movement component inactive
    // path follow
    bee->AddComponent<PathMovement>(bee.get(), path, 355.f, true);
    const auto pathFollow = bee->GetComponent<PathMovement>();
    pathFollow->AddWorldSpacePoint({ x, y });
    pathFollow->StartAtFirstPoint();
    pathFollow->OnPathCompleted.AddListener(this, &WaveManager::OnEnemyPathComplete);
    // Health
    bee->AddComponent<Health>(bee.get());
    bee->GetComponent<Health>()->OnDeath.AddListener(this, &WaveManager::OnEnemyKilled);
    // Collider
    bee->AddComponent<dae::ColliderComponent>(bee.get(), glm::vec2(28.f, 28.f));
    bee->AddComponent<EnemyCollisionComponent>(bee.get());
    //bee->AddComponent<dae::ColliderRenderComponent>(bee.get());
    // Attack
    bee->AddComponent<BeeAttackBehavior>(bee.get());
    bee->GetComponent<BeeAttackBehavior>()->OnAttackCompleted.AddListener(this, &WaveManager::OnEnemyAttackComplete);
    bee->GetComponent<BeeAttackBehavior>()->SetActive(false);

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
    movement->ResetPosition({ static_cast<float>(x), static_cast<float>(y) });
    movement->SetActive(false); // Set movement component inactive
    // path follow
    butterfly->AddComponent<PathMovement>(butterfly.get(), path, 355.f, true);
    const auto pathFollow = butterfly->GetComponent<PathMovement>();
    pathFollow->AddWorldSpacePoint({ x, y });
    pathFollow->StartAtFirstPoint();
    pathFollow->OnPathCompleted.AddListener(this, &WaveManager::OnEnemyPathComplete);
    // Health
    butterfly->AddComponent<Health>(butterfly.get());
    butterfly->GetComponent<Health>()->OnDeath.AddListener(this, &WaveManager::OnEnemyKilled);
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
    bossGalaga->AddComponent<dae::AnimatorComponent>(bossGalaga.get(), 2, 2, 1.f, true);
    bossGalaga->GetComponent<dae::AnimatorComponent>()->SetManualFrames({ 0,1 });
    // movement
    bossGalaga->AddComponent<BasicEnemyMovementBehavior>(bossGalaga.get(), 35.f);
    const auto movement = bossGalaga->GetComponent<BasicEnemyMovementBehavior>();
    movement->SetBounds(x - moveDistance, x + moveDistance);
    movement->ResetPosition({ static_cast<float>(x), static_cast<float>(y) });
    movement->SetActive(false); // Set movement component inactive
    // path follow
    bossGalaga->AddComponent<PathMovement>(bossGalaga.get(), path, 355.f, true);
    const auto pathFollow = bossGalaga->GetComponent<PathMovement>();
    pathFollow->AddWorldSpacePoint({ x, y });
    pathFollow->StartAtFirstPoint();
    pathFollow->OnPathCompleted.AddListener(this, &WaveManager::OnEnemyPathComplete);
    // Health
    bossGalaga->AddComponent<Health>(bossGalaga.get(), 2);
    bossGalaga->GetComponent<Health>()->OnDeath.AddListener(this, &WaveManager::OnEnemyKilled);
    // boss
    bossGalaga->AddComponent<BossGalaga>(bossGalaga.get());
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