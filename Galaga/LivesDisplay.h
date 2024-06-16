#pragma once
#include "GameObject.h"

namespace galaga
{
	class LivesDisplay final :public dae::BaseComponent
	{
	public:
		LivesDisplay(dae::GameObject* gameObject);
		~LivesDisplay() = default;

	private:
		void OnLivesChanged(int score);
		void InstantiateIcon(float xPos);
	};
}
