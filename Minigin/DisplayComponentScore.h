#pragma once
#include "BaseComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Event.h"

namespace galaga
{
    class DisplayComponentScore final : public dae::BaseComponent, public dae::EventListener
    {
    public:
        DisplayComponentScore(dae::GameObject* gameObject, ScoreComponent* scoreComponent);
        ~DisplayComponentScore() = default;

        DisplayComponentScore(const DisplayComponentScore& other) = delete;
        DisplayComponentScore(DisplayComponentScore&& other) = delete;
        DisplayComponentScore& operator=(const DisplayComponentScore& other) = delete;
        DisplayComponentScore& operator=(DisplayComponentScore&& other) = delete;

        void OnScoreChanged(int currentScore);

    private:
        std::string m_BaseText{ "score: " };
        dae::TextComponent* m_TextComponent{};
    };
}
