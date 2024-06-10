#pragma once

#include "BaseComponent.h"
#include "Event.h"
#include <vector>
#include <glm/glm.hpp>

namespace dae
{
    class TransformComponent;
}

namespace galaga
{
    class PathMovement : public dae::BaseComponent
    {
    public:
        PathMovement(dae::GameObject* gameObject, const std::vector<glm::vec2>& path, float speed = 100.0f, bool rotateAlongPath = false);

        void Update() override;
        bool IsPathComplete() const;
        void SetRotateAlongPath(bool rotate);
        dae::Event<> OnPathCompleted;

    private:
        void MoveAlongPath();
        glm::vec2 ConvertToWorldCoordinates(const glm::vec2& relativePos);

        dae::TransformComponent* m_TransformComponent;
        std::vector<glm::vec2> m_Path;
        float m_Speed;
        size_t m_CurrentPoint;
        bool m_PathComplete;
        bool m_RotateAlongPath;
    };
}
