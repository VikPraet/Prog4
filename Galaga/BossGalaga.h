#pragma once
#include "BaseComponent.h"

namespace galaga
{
	class Health;

	class BossGalaga final : public dae::BaseComponent
	{
	public:
		BossGalaga(dae::GameObject* gameObject);
		~BossGalaga() = default;

		void Update() override;
		void OnHit();

	private:
		Health* m_Health;
	};
}
