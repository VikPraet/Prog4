#include "AnimatorComponent.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "RenderComponent.h"
#include "GameTime.h"

dae::AnimatorComponent::AnimatorComponent(dae::GameObject* gameObject, int rows, int columns, float framesPerSec, bool syncWithRunningTime)
    : BaseComponent(gameObject), m_Rows(rows), m_Columns(columns), m_FramesPerSecond(framesPerSec), m_SyncWithRunningTime(syncWithRunningTime)
{
    m_SecondsPerFrame = 1.f / m_FramesPerSecond;
    m_TotalFrames = m_Rows * m_Columns;

    const auto renderComponent = gameObject->GetComponent<RenderComponent>();
    if (renderComponent)
    {
        m_Texture = renderComponent->GetTexture();
    }
}

void dae::AnimatorComponent::Update()
{
    if (m_SyncWithRunningTime)
    {
        m_CurrentFrame = static_cast<int>(GameTime::GetInstance().GetRunningTime() / m_SecondsPerFrame) % m_TotalFrames;
    }
    else
    {
        m_ElapsedTime += GameTime::GetInstance().GetDeltaTime();
        if (m_ElapsedTime >= m_SecondsPerFrame)
        {
            m_ElapsedTime -= m_SecondsPerFrame;
            m_CurrentFrame = (m_CurrentFrame + 1) % m_TotalFrames;
        }
    }
}

void dae::AnimatorComponent::SetFramesPerSecond(float framesPerSecond)
{
    m_FramesPerSecond = framesPerSecond;
    m_SecondsPerFrame = 1.0f / framesPerSecond;
}

SDL_Rect dae::AnimatorComponent::GetCurrentFrameRect() const
{
    SDL_Rect srcRect{};
    const int frameWidth = m_Texture->GetSize().x / m_Columns;
    const int frameHeight = m_Texture->GetSize().y / m_Rows;
    const int currentRow = m_CurrentFrame / m_Columns;
    const int currentColumn = m_CurrentFrame % m_Columns;

    srcRect.x = currentColumn * frameWidth;
    srcRect.y = currentRow * frameHeight;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;

    return srcRect;
}
