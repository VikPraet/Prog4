#include "LifeTime.h"

#include "GameObject.h"
#include "GameTime.h"

galaga::LifeTime::LifeTime(dae::GameObject* gameObject, float lifeTime)
	: BaseComponent(gameObject), m_Lifetime(lifeTime)
{
}

void galaga::LifeTime::Update()
{
    m_Lifetime -= dae::GameTime::GetInstance().GetDeltaTime();
    if (m_Lifetime <= 0.0f)
    {
        GetGameObject()->Destroy();
        OnLifeTimeFinished.Invoke();
    }
}
