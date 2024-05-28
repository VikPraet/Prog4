#pragma once
#include "Command.h"

namespace galaga
{
	class MoveCommand final : public dae::GameObjectCommand
	{
	public:
		MoveCommand(dae::GameObject* gameObject, const glm::vec2& direction)
			: GameObjectCommand(gameObject), m_Direction(direction)
		{
			m_TransformComponent = GetGameObject()->GetComponent<dae::TransformComponent>();
		}

		void Execute() override
		{
			m_TransformComponent->Translate(m_Direction.x * dae::GameTime::GetInstance().GetDeltaTime(), m_Direction.y * dae::GameTime::GetInstance().GetDeltaTime(), 0);
		}

	private:
		glm::vec2 m_Direction;
		dae::TransformComponent* m_TransformComponent;
	};

	class DamageCommand final : public dae::GameObjectCommand
	{
	public:
		DamageCommand(dae::GameObject* gameObject, float damage)
			: GameObjectCommand(gameObject), m_DamageAmount(damage)
		{
			m_HealthComponent = GetGameObject()->GetComponent<HealthComponent>();
		}

		void Execute() override
		{
			m_HealthComponent->DecreaseHealth(m_DamageAmount);
		}

	private:
		float m_DamageAmount;
		HealthComponent* m_HealthComponent;
	};

	class ScoreGainCommand final : public dae::GameObjectCommand
	{
	public:
		ScoreGainCommand(dae::GameObject* gameObject, int score)
			: GameObjectCommand(gameObject), m_ScoreAmount(score)
		{
			m_ScoreComponent = GetGameObject()->GetComponent<ScoreComponent>();
		}

		void Execute() override
		{
			m_ScoreComponent->IncreaseScore(m_ScoreAmount);
		}

	private:
		int m_ScoreAmount;
		ScoreComponent* m_ScoreComponent;
	};
}
