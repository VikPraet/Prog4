#pragma once
#include "GameObject.h"

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
		GameObjectCommand(GameObject* gameObject)
			: m_GameObject(gameObject)
		{
		}

		virtual ~GameObjectCommand() = default;

	protected:
		GameObject* GetGameObject() const { return m_GameObject; }

	private:
		GameObject* m_GameObject;
	};

	class GameObjectStickCommand : public GameObjectCommand {
	public:
		GameObjectStickCommand(GameObject* gameObject)
			: GameObjectCommand(gameObject)
		{
		}

		virtual ~GameObjectStickCommand() = default;

		void Execute() override {}
		virtual void Execute(float x, float y) = 0;
	};
}
