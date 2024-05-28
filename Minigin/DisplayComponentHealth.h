#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "TextComponent.h"

namespace galaga
{
	class DisplayComponentHealth final : public dae::BaseComponent, public dae::Observer
	{
    public:
        DisplayComponentHealth(dae::GameObject* gameObject);
        ~DisplayComponentHealth() = default;

        DisplayComponentHealth(const DisplayComponentHealth& other) = delete;
        DisplayComponentHealth(DisplayComponentHealth&& other) = delete;
        DisplayComponentHealth& operator=(const DisplayComponentHealth& other) = delete;
        DisplayComponentHealth& operator=(DisplayComponentHealth&& other) = delete;

        void Notify(const dae::Event& event, dae::Subject* subject) override;

    private:
        void UpdateHealthDisplay(dae::Subject* subject);
        void UpdateScoreDisplay(dae::Subject* subject);

        std::string m_BaseText{ "# Lives: " };
        dae::TextComponent* m_TextComponent{};
	};
}
