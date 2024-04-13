#include "FpsComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"

FpsComponent::FpsComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_TextComponent(nullptr)
{
}

void FpsComponent::Update()
{
    if (m_TextComponent == nullptr) m_TextComponent = GetGameObject()->GetComponent<TextComponent>();

    ++m_Count;
    m_AddedDeltaTimes += GameTime::GetInstance().GetUnScaledDeltaTime();
    const auto currentTime = std::chrono::steady_clock::now();

    if (const auto elapsedSeconds = std::chrono::duration<float>(currentTime - m_lastUpdateTime).count(); elapsedSeconds >= m_TimeBetweenUpdates)
    {
        m_lastUpdateTime = currentTime;

        std::stringstream ss;
        ss << "FPS: " << std::fixed << std::setprecision(1) << 1 / (m_AddedDeltaTimes / m_Count);

        m_text = ss.str();

        m_Count = 0;
        m_AddedDeltaTimes = 0;

        if (const auto gameObject = GetGameObject())
        {
        	if(m_TextComponent) m_TextComponent->SetText(m_text);
        }
    }
}
