#include "FpsComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"

FpsComponent::FpsComponent(const std::shared_ptr<dae::GameObject>& gameObject)
	: Component(gameObject)
{
}

void FpsComponent::Update()
{
    ++m_Count;
    m_AddedDeltaTimes += GameTime::GetInstance().GetDeltaTime();
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
            if (m_TextComponent == nullptr)
                m_TextComponent = GetGameObject()->GetComponent<TextComponent>();
            else
                m_TextComponent->SetText(m_text);
        }
    }
}
