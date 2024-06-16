#pragma once
#include "Event.h"
#include "GameObject.h"
#include "SingletonComponent.h"

namespace galaga
{
	class LivesManager final : public dae::SingletonComponent<LivesManager>
	{
	public:
		LivesManager(dae::GameObject* gameObject = nullptr);
		~LivesManager() = default;

		void LoseLife(int amount);
		int GetLives() const { return m_Lives; }

		dae::Event<int> OnLivesChanged;

	private:
		int m_Lives{ 3 };
	};
}
