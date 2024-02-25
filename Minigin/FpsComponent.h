#pragma once
#include <chrono>

#include "Component.h"

#include <string>

class FpsComponent final : public Component
{
public:
    FpsComponent() = default;
    FpsComponent(const std::weak_ptr<dae::GameObject>& pGameObject);
    ~FpsComponent() = default;

    void Update() override;

protected:
    float m_TimeBetweenUpdates = 0.07f; // seconds
    std::chrono::steady_clock::time_point m_lastUpdateTime;
    std::string m_text;
    std::weak_ptr<dae::GameObject> m_GameObject;
};
