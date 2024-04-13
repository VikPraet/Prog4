#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
	class DisplayComponentHealth final : public BaseComponent, public Observer
	{
    public:
        DisplayComponentHealth(dae::GameObject* gameObject);
        ~DisplayComponentHealth() = default;

        DisplayComponentHealth(const DisplayComponentHealth& other) = delete;
        DisplayComponentHealth(DisplayComponentHealth&& other) = delete;
        DisplayComponentHealth& operator=(const DisplayComponentHealth& other) = delete;
        DisplayComponentHealth& operator=(DisplayComponentHealth&& other) = delete;

        void Notify(const Event& event, Subject* subject) override;

    private:
        void UpdateHealthDisplay(Subject* subject);
        void UpdateScoreDisplay(Subject* subject);

        std::string m_BaseText{ "# Lives: " };
        TextComponent* m_TextComponent{};
	};
}
