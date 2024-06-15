#include "PlayerHealth.h"
#include "GameObject.h"
#include "TransformComponent.h"

galaga::PlayerHealth::PlayerHealth(dae::GameObject* gameObject)
	: Health(gameObject)
{
}

void galaga::PlayerHealth::Hit(int damage)
{
	m_Health -= damage;
	OnHit.Invoke();

	if (m_Health <= 0)
	{
		m_Health = 0;
		OnDeath.Invoke(GetGameObject());
		OnPlayerDeath.Invoke(GetGameObject());

		GetGameObject()->SetTag("");
		GetGameObject()->GetComponent<dae::TransformComponent>()->SetWorldPosition(-1000, -1000);
		GetGameObject()->GetComponent<dae::TransformComponent>()->SetRotation(0.f);
		GetGameObject()->SetActive(false);

	}
}
