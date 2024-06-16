#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"

namespace galaga
{
	class ScoreDisplay final : public dae::BaseComponent
	{
	public:
		ScoreDisplay(dae::GameObject* gameObject);
		~ScoreDisplay() = default;

	private:
		void OnScoreChanged(int score);
		void OnComponentAdded(BaseComponent* component) override;

		dae::TextComponent* m_ScoreTextComponent;
	};
}
