#include "TractorBeam.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "RenderComponent.h"
#include "AnimatorComponent.h"
#include "GameTime.h"
#include "InputManager.h"
#include "LifeTime.h"
#include "PathMovement.h"
#include "RotationComponent.h"


#include "SceneManager.h"

galaga::TractorBeam::TractorBeam(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_ColliderComponent(nullptr), m_AnimatorComponent(nullptr)
{
    if(!m_AnimatorComponent) m_AnimatorComponent = GetGameObject()->GetComponent<dae::AnimatorComponent>();

    if(!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<dae::ColliderComponent>();
        if (m_ColliderComponent) m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &TractorBeam::OnTriggerEnter);
    }

}

void galaga::TractorBeam::Update()
{
    if(!m_AnimatorComponent) m_AnimatorComponent = GetGameObject()->GetComponent<dae::AnimatorComponent>();

    if (!m_ColliderComponent)
    {
        m_ColliderComponent = GetGameObject()->GetComponent<dae::ColliderComponent>();
        if (m_ColliderComponent) m_ColliderComponent->OnTriggerEnterEvent.AddListener(this, &TractorBeam::OnTriggerEnter);
    }


    if(!m_IsActive && m_AnimatorComponent)
    {
        if (m_AnimatorComponent->GetCurrentFrame() == 17)
        {
            m_IsActive = true;
            m_AnimatorComponent->SetManualFrames({ 15, 16, 17 });
            GetGameObject()->AddComponent<dae::ColliderComponent>(GetGameObject(), glm::vec2{ 96, 160 });
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
                m_AnimatorComponent->SetManualFrames({ 14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 });

                GetGameObject()->RemoveComponent<dae::ColliderComponent>();
				GetGameObject()->AddComponent<LifeTime>(GetGameObject(), 1.55f);
                if (const auto lifeTime = GetGameObject()->GetComponent<LifeTime>()) lifeTime->OnLifeTimeFinished.AddListener(this, &TractorBeam::OnDestroy);
            }
        }
    }
}

void galaga::TractorBeam::OnTriggerEnter(dae::GameObject* self, dae::GameObject* other)
{
    if (other->GetTag() != "player") return;

    //other->Destroy();
    dae::InputManager::GetInstance().ResetGamepadBindings(0);
    dae::InputManager::GetInstance().ResetAllBindings();

    m_Player = other;

    // make untagged
    other->SetTag("");

    // disable collider so that no damage is taken / done
    other->RemoveComponent<dae::ColliderComponent>();

    // add path movement component to move player into tracktor beam
    if(const auto parentTransform = self->GetParent()->GetComponent<dae::TransformComponent>())
    {
        other->AddComponent<PathMovement>(other);
        if(const auto pathmovement = other->GetComponent<PathMovement>())
        {
	        const std::vector<glm::vec2> path{
            { parentTransform->GetWorldPosition().x, parentTransform->GetWorldPosition().y + 80},
            { parentTransform->GetWorldPosition().x, parentTransform->GetWorldPosition().y - 40 }
            };

            pathmovement->SetSpeed(100.f);
            pathmovement->SetWorldSpacePath(path);

            pathmovement->OnPathCompleted.AddListener(this, &TractorBeam::OnPathComplete);
        }
    }

    // add rotation component so make player rotate
    other->AddComponent<dae::RotationComponent>(other);
    other->GetComponent<dae::RotationComponent>()->SetRotateAroundSelf(true);
    other->GetComponent<dae::RotationComponent>()->SetRotatetionSpeed(750.f);
}

void galaga::TractorBeam::OnDestroy()
{
    OnTractorCompleted.Invoke();
}

void galaga::TractorBeam::OnPathComplete()
{
    m_Player->Destroy();
    SpawnEnemyShip();
}

void galaga::TractorBeam::SpawnEnemyShip()
{
	auto enemyShip = std::make_unique<dae::GameObject>();

    enemyShip->AddComponent<dae::TransformComponent>(enemyShip.get());
    enemyShip->GetComponent<dae::TransformComponent>()->SetScale(2);

    enemyShip->AddComponent<dae::RenderComponent>(enemyShip.get());
    enemyShip->GetComponent<dae::RenderComponent>()->SetTexture("galaga-enemy-fighter.png");

    enemyShip->SetParent(GetGameObject()->GetParent());
    enemyShip->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, -40);

    dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(enemyShip));
}
