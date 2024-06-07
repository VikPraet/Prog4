#pragma once
#include "BaseComponent.h"
#include <SDL_rect.h>

namespace dae
{
    class Texture2D;
    class RenderComponent;

    class AnimatorComponent final : public BaseComponent
    {
    public:
        AnimatorComponent(GameObject* gameObject, int rows = 1, int columns = 1, float framesPerSec = 10.f);
        ~AnimatorComponent() = default;

        void Update() override;

        void SetFramesPerSecond(float framesPerSecond);
        SDL_Rect GetCurrentFrameRect() const;

    private:
        Texture2D* m_Texture{ nullptr };
        int m_Rows{};
        int m_Columns{};
        int m_TotalFrames{};
        int m_CurrentFrame{};
        float m_FramesPerSecond{};
        float m_SecondsPerFrame{};
        float m_ElapsedTime{};
    };
}