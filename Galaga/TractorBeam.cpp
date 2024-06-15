#include "TractorBeam.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "RenderComponent.h"
#include "AnimatorComponent.h"
#include "BossAttackBehavior.h"
#include "EnemyFighter.h"
#include "GameTime.h"
#include "PathMovement.h"
#include "InputManager.h"
#include "LifeTime.h"
#include "PlayerHealth.h"
#include "PlayerMovementBehavior.h"
#include "RotationComponent.h"

#include "SceneManager.h"

galaga::TractorBeam::TractorBeam(dae::GameObject* gameObject)
    : BaseComponent(gameObject), m_ColliderComponent(nullptr), m_AnimatorComponent(nullptr), m_Player(nullptr)
{
    if(!m_AnimatorComponent) m_AnimatorComponent = GetGameObject()->GetComponent<dae::AnimatorComponent>();

    if(!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<dae::ColliderComponent>();
        if (m_ColliderComponent)
        {
            m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &TractorBeam::OnTriggerEnter);
            m_ColliderComponent->SetActive(false);
        }
    }

}

void galaga::TractorBeam::Update()
{
    if(!m_AnimatorComponent) m_AnimatorComponent = GetGameObject()->GetComponent<dae::AnimatorComponent>();

    if (!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<dae::ColliderComponent>();
        if (m_ColliderComponent)
        {
            m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &TractorBeam::OnTriggerEnter);
            m_ColliderComponent->SetActive(false);
        }
    }

    if(!m_IsActive && m_AnimatorComponent)
    {
        if (m_AnimatorComponent->GetCurrentFrame() == 17)
        {
            m_IsActive = true;
            m_AnimatorComponent->SetManualFrames({ 15, 16, 17 });
            m_ColliderComponent->SetActive(true);
            //GetGameObject()->AddComponent<dae::ColliderComponent>(GetGameObject(), glm::vec2{ 96, 160 });
        }
    }

    if(m_IsActive)
    {
        m_AccuTime += dae::GameTime::GetInstance().GetDeltaTime();
        if(m_AccuTime >= m_ActiveTime)
        {
            if (!m_IsRetracting && m_AnimatorComponent)
            {
                m_IsRetracting = true;
                m_AnimatorComponent->SetManualFrames({ 14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0 });
                m_ColliderComponent->SetActive(false);
				GetGameObject()->AddComponent<LifeTime>(GetGameObject(), 1.65f);
                if (const auto lifeTime = GetGameObject()->GetComponent<LifeTime>()) lifeTime->OnLifeTimeFinished.AddListener(this, &TractorBeam::OnDestroy);
            }
        }
    }
}

void galaga::TractorBeam::OnTriggerEnter(dae::GameObject* self, dae::GameObject* other)
{
    if (other->GetTag() != "player") return;

    m_Player = other;

    // make untagged
    other->SetTag("");

    // disable collider so that no damage is taken / done
    other->GetComponent<dae::ColliderComponent>()->SetActive(false);

    self->GetParent()->GetComponent<BossAttackBehavior>()->SetHasCaughtPlayer(true);

    SpawnEnemyShip(self->GetParent());
    m_Player->GetComponent<PlayerHealth>()->OnPlayerDeath.Invoke(m_Player);
}

void galaga::TractorBeam::OnDestroy()
{
    OnTractorCompleted.Invoke();
}

void galaga::TractorBeam::SpawnEnemyShip(dae::GameObject* parent)
{
	auto enemyShip = std::make_unique<dae::GameObject>();
    // add transform component
    enemyShip->AddComponent<dae::TransformComponent>(enemyShip.get());
    enemyShip->GetComponent<dae::TransformComponent>()->SetScale(2);
    // arr render component
    enemyShip->AddComponent<dae::RenderComponent>(enemyShip.get());
    enemyShip->GetComponent<dae::RenderComponent>()->SetTexture("galaga-fighter.png");
    // add path movement
    enemyShip->AddComponent<PathMovement>(enemyShip.get());
    // add rotationComponent
    enemyShip->AddComponent<dae::RotationComponent>(enemyShip.get());
    // add enemy fighter component
    enemyShip->AddComponent<EnemyFighter>(enemyShip.get());
    // set parent and adjust transform
    enemyShip->SetParent(GetGameObject()->GetParent());

	const auto parentTransform = parent->GetComponent<dae::TransformComponent>();
	const auto PlayerTransform = m_Player->GetComponent<dae::TransformComponent>();
    if (parentTransform && PlayerTransform)
    {
        enemyShip->GetComponent<dae::TransformComponent>()->SetWorldPosition(PlayerTransform->GetWorldPosition().x, PlayerTransform->GetWorldPosition().y);
        const std::vector<glm::vec2> path{
	    			 { PlayerTransform->GetWorldPosition().x ,PlayerTransform->GetWorldPosition().y },
                     { parentTransform->GetWorldPosition().x, parentTransform->GetWorldPosition().y + 80 },
                     { parentTransform->GetWorldPosition().x, parentTransform->GetWorldPosition().y - 40 }
                 };

        enemyShip->GetComponent<PathMovement>()->SetSpeed(100.f);
        enemyShip->GetComponent<PathMovement>()->SetWorldSpacePath(path);
    }

	if(const auto rotatingComponent = enemyShip->GetComponent<dae::RotationComponent>())
	{
        rotatingComponent->SetRotateAroundSelf(true);
        rotatingComponent->SetRotatetionSpeed(750.f);
	}

    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(enemyShip));
}
