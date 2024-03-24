#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class ScoreComponent : public BaseComponent, public dae::Subject
	{
	public:
		ScoreComponent(const std::shared_ptr<dae::GameObject>& gameObject);
		~ScoreComponent() = default;

		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void IncreaseScore(int amount = 1.f);
		int GetCurrentScore() const { return m_CurrentScore; }

	private:
		int m_CurrentScore{};
	};
}
