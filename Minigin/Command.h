#pragma once
#include "GameObject.h"
#include "GameTime.h"
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
}
