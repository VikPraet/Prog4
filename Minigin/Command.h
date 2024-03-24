#pragma once
#include "GameObject.h"
#include "GameTime.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "TransformComponent.h"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(const std::shared_ptr<dae::GameObject>& gameObject)
			: m_GameObject(gameObject)
		{
		}

		virtual ~GameObjectCommand() = default;

	protected:
		std::shared_ptr<dae::GameObject> GetGameObject() const { return std::shared_ptr(m_GameObject); }

	private:
		std::weak_ptr<GameObject> m_GameObject;
	};

	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(const std::shared_ptr<dae::GameObject>& gameObject, const glm::vec2& direction)
			: GameObjectCommand(gameObject), m_Direction(direction)
		{
			m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();
		}

		void Execute() override
		{
			m_TransformComponent->Translate(m_Direction.x * GameTime::GetInstance().GetDeltaTime(), m_Direction.y * GameTime::GetInstance().GetDeltaTime(), 0);
		}

	private:
		glm::vec2 m_Direction;
		TransformComponent* m_TransformComponent;
	};

	class DamageCommand final : public GameObjectCommand
	{
	public:
		DamageCommand(const std::shared_ptr<dae::GameObject>& gameObject, float damage)
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

	class ScoreGainCommand final : public GameObjectCommand
	{
	public:
		ScoreGainCommand(const std::shared_ptr<dae::GameObject>& gameObject, int score)
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
