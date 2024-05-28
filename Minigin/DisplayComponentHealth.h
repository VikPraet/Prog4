#pragma once
#include "BaseComponent.h"
#include "Event.h"
#include "HealthComponent.h"
#include "TextComponent.h"

namespace galaga
{
	class DisplayComponentHealth final : public dae::BaseComponent, public dae::EventListener
	{
    public:
        DisplayComponentHealth(dae::GameObject* gameObject, HealthComponent* healthComponent);
        ~DisplayComponentHealth() = default;

        DisplayComponentHealth(const DisplayComponentHealth& other) = delete;
        DisplayComponentHealth(DisplayComponentHealth&& other) = delete;
        DisplayComponentHealth& operator=(const DisplayComponentHealth& other) = delete;
        DisplayComponentHealth& operator=(DisplayComponentHealth&& other) = delete;

        void OnHealthChanged(float currentHealth);

    private:
        std::string m_BaseText{ "# Lives: " };
        dae::TextComponent* m_TextComponent{};
	};
}
