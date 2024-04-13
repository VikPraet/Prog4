#pragma once

#include "BaseComponent.h"

#include <chrono>
#include <string>

class TextComponent;

class FpsComponent final : public BaseComponent
{
public:
    FpsComponent(dae::GameObject* gameObject);
    ~FpsComponent() = default;

    void Update() override;

protected:
    float m_TimeBetweenUpdates = 0.07f; // seconds
    std::chrono::steady_clock::time_point m_lastUpdateTime;
    std::string m_text;
    TextComponent* m_TextComponent;

    float m_AddedDeltaTimes;
    int m_Count;
};
