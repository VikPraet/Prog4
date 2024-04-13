#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
    class DisplayComponentScore final : public BaseComponent, public Observer
    {
    public:
        DisplayComponentScore(dae::GameObject* gameObject);
        ~DisplayComponentScore() = default;

        DisplayComponentScore(const DisplayComponentScore& other) = delete;
        DisplayComponentScore(DisplayComponentScore&& other) = delete;
        DisplayComponentScore& operator=(const DisplayComponentScore& other) = delete;
        DisplayComponentScore& operator=(DisplayComponentScore&& other) = delete;

        void Notify(const Event& event, Subject* subject) override;

    private:
        void UpdateHealthDisplay(Subject* subject);
        void UpdateScoreDisplay(Subject* subject);

        std::string m_BaseText{ "score: " };
        TextComponent* m_TextComponent{};
    };
}
