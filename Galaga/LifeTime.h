#pragma once
#include "BaseComponent.h"

namespace galaga
{
	class LifeTime final : public dae::BaseComponent
	{
	public:
		LifeTime(dae::GameObject* gameObject, float lifeTime = 1.f);
		~LifeTime() = default;

		void Update() override;

		void SetLifeTime(float lifeTime) { m_Lifetime = lifeTime; }
		float GetLifeTime() const { return m_Lifetime; }

	private:
		float m_Lifetime;
	};
}
