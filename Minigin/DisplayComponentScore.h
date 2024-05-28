#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "TextComponent.h"

namespace galaga
{
    class DisplayComponentScore final : public dae::BaseComponent, public dae::Observer
    {
    public:
        DisplayComponentScore(dae::GameObject* gameObject);
        ~DisplayComponentScore() = default;

        DisplayComponentScore(const DisplayComponentScore& other) = delete;
        DisplayComponentScore(DisplayComponentScore&& other) = delete;
        DisplayComponentScore& operator=(const DisplayComponentScore& other) = delete;
        DisplayComponentScore& operator=(DisplayComponentScore&& other) = delete;

        void Notify(const dae::Event& event, dae::Subject* subject) override;

    private:
        void UpdateHealthDisplay(dae::Subject* subject);
        void UpdateScoreDisplay(dae::Subject* subject);

        std::string m_BaseText{ "score: " };
        dae::TextComponent* m_TextComponent{};
    };
}
