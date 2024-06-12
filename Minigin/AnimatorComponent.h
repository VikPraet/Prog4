#pragma once
#include "BaseComponent.h"
#include <SDL_rect.h>
#include <vector>

namespace dae
{
    class Texture2D;
    class RenderComponent;

    class AnimatorComponent final : public BaseComponent
    {
    public:
        AnimatorComponent(GameObject* gameObject, int rows = 1, int columns = 1, float framesPerSec = 10.f, bool syncWithRunningTime = false);
        ~AnimatorComponent() = default;

        void Update() override;

        void SetFramesPerSecond(float framesPerSecond);
        SDL_Rect GetCurrentFrameRect() const;

        void SetCurrentFrame(int frame) { m_CurrentFrame = frame % m_TotalFrames; }
        int GetCurrentFrame() const { return m_CurrentFrame; }

        void SetSyncWithRunningTime(bool sync) { m_SyncWithRunningTime = sync; }

        // New methods
        void SetManualFrames(const std::vector<int>& frames);
        void ClearManualFrames();

    private:
        Texture2D* m_Texture{ nullptr };
        int m_Rows{};
        int m_Columns{};
        int m_TotalFrames{};
        int m_CurrentFrame{};
        float m_FramesPerSecond{};
        float m_SecondsPerFrame{};
        float m_ElapsedTime{};
        bool m_SyncWithRunningTime{ false };

        // New member variable
        std::vector<int> m_ManualFrames;
        size_t m_CurrentManualFrameIndex{ 0 };
    };
}
