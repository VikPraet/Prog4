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
		GameObjectCommand(dae::GameObject* gameObject)
			: m_GameObject(gameObject)
		{
		}

		virtual ~GameObjectCommand() = default;

	protected:
		dae::GameObject* GetGameObject() const { return m_GameObject; }

	private:
		dae::GameObject* m_GameObject;
	};
}
