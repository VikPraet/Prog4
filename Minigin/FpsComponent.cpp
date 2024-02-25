#include "FpsComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"

FpsComponent::FpsComponent(const std::weak_ptr<dae::GameObject>& pGameObject)
	: Component(), m_GameObject(pGameObject)
{
}

void FpsComponent::Update()
{
    const auto currentTime = std::chrono::steady_clock::now();
    if (const auto elapsedSeconds = std::chrono::duration<float>(currentTime - m_lastUpdateTime).count(); elapsedSeconds >= m_TimeBetweenUpdates)
    {
        m_lastUpdateTime = currentTime;

        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << 1 / GameTime::GetInstance().GetDeltaTime();

        m_text = "FPS: " + ss.str();

        if (const auto gameObject = m_GameObject.lock())
        {
	        if (const auto textComponent = gameObject->GetComponent<TextComponent>(); textComponent != nullptr)
            {
                textComponent->SetText(m_text);
            }
        }
    }
}
