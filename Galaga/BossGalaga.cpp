#include "BossGalaga.h"

#include "AnimatorComponent.h"
#include "GameObject.h"
#include "Health.h"

galaga::BossGalaga::BossGalaga(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_Health(nullptr)
{
}

void galaga::BossGalaga::Update()
{
	if (!m_Health)
	{
		m_Health = GetGameObject()->GetComponent<Health>();
		m_Health->OnHit.AddListener(this, &BossGalaga::OnHit);
	}
}

void galaga::BossGalaga::OnHit()
{
	if(const auto animator = GetGameObject()->GetComponent<dae::AnimatorComponent>())
	{
		animator->SetManualFrames({2,3});
	}
}
