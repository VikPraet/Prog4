#include "EnemyFighter.h"

#include "BossAttackBehavior.h"
#include "Bullet.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "TransformComponent.h"

galaga::EnemyFighter::EnemyFighter(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_Transform(nullptr), m_ParentTransform(nullptr), m_PathMovement(nullptr), m_RotationComponent(nullptr)
{
	if (!m_Transform) m_Transform = gameObject->GetComponent<dae::TransformComponent>();
	if (!m_ParentTransform)
	{
		if (const auto parent = gameObject->GetParent()) m_Transform = parent->GetComponent<dae::TransformComponent>();
	}

	if (!m_PathMovement)
	{
		m_PathMovement = GetGameObject()->GetComponent<PathMovement>();
		m_PathMovement->OnPathCompleted.AddListener(this, &EnemyFighter::OnPathCompleted);
	}

	if (!m_RotationComponent) m_RotationComponent = GetGameObject()->GetComponent<dae::RotationComponent>();

	if (!m_HasSubscribedToEvent)
	{
		if (const auto parent = GetGameObject()->GetParent())
			if (const auto attackBehavior = parent->GetComponent<BossAttackBehavior>())
			{
				attackBehavior->OnShoot.AddListener(this, &EnemyFighter::Shoot);
				m_HasSubscribedToEvent = true;
			}
	}
}

void galaga::EnemyFighter::Update()
{
	if (!m_Transform) m_Transform = GetGameObject()->GetComponent<dae::TransformComponent>();
	if (!m_ParentTransform)
	{
		if (const auto parent = GetGameObject()->GetParent()) m_ParentTransform = parent->GetComponent<dae::TransformComponent>();
	}

	if (!m_PathMovement)
	{
		m_PathMovement = GetGameObject()->GetComponent<PathMovement>();
		m_PathMovement->OnPathCompleted.AddListener(this, &EnemyFighter::OnPathCompleted);
	}

	if (!m_RotationComponent) m_RotationComponent = GetGameObject()->GetComponent<dae::RotationComponent>();

	if (!m_HasSubscribedToEvent)
	{
		if (const auto parent = GetGameObject()->GetParent())
			if (const auto attackBehavior = parent->GetComponent<BossAttackBehavior>())
			{
				attackBehavior->OnShoot.AddListener(this, &EnemyFighter::Shoot);
				m_HasSubscribedToEvent = true;
			}
	}

	// set rotation to rotation of parent
	if(!m_RotationComponent->GetActive() && m_ParentTransform && m_Transform) m_Transform->SetRotation(m_ParentTransform->GetRotation());
}

void galaga::EnemyFighter::OnPathCompleted()
{
	m_RotationComponent->SetActive(false);
	m_Transform->SetRotation(0.f);
	GetGameObject()->GetComponent<dae::RenderComponent>()->SetTexture("galaga-enemy-fighter.png");
	GetGameObject()->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, -40);

	dae::ServiceLocator::GetService<ISoundService>()->PlaySoundEffect("Fighter-Captured.wav");
}

void galaga::EnemyFighter::Shoot()
{
	const auto& players = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("player");

	if (players.empty()) return;

	auto bulletObject = std::make_unique<dae::GameObject>();

	bulletObject->AddComponent<dae::TransformComponent>(bulletObject.get());
	bulletObject->GetComponent<dae::TransformComponent>()->SetScale(2);

	bulletObject->AddComponent<dae::RenderComponent>(bulletObject.get());
	bulletObject->GetComponent<dae::RenderComponent>()->SetTexture("enemy-bullet.png");

	const auto playerPosition = players[0]->GetComponent<dae::TransformComponent>()->GetWorldPosition();
	const auto enemyPosition = m_Transform->GetWorldPosition();
	glm::vec2 direction = normalize(glm::vec2(playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y));

	bulletObject->AddComponent<Bullet>(bulletObject.get(), 300.f, 3.f, direction);
	bulletObject->AddComponent<dae::ColliderComponent>(bulletObject.get(), glm::vec2(6.f, 14.f));
	bulletObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(enemyPosition);
	bulletObject->SetTag("enemyBullet");

	dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(bulletObject));
}
