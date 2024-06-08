#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include <vector>
#include <glm/vec2.hpp>

namespace galaga
{
    class PathMovement : public dae::BaseComponent
    {
    public:
        PathMovement(dae::GameObject* gameObject, const std::vector<glm::vec2>& path, float speed = 100.0f, bool rotateAlongPath = true);
        ~PathMovement() = default;

        void Update() override;
        bool IsPathComplete() const;
        void SetRotateAlongPath(bool rotate);

    private:
        dae::TransformComponent* m_TransformComponent;
        std::vector<glm::vec2> m_Path;
        float m_Speed;
        size_t m_CurrentPoint;
        bool m_PathComplete;
        bool m_RotateAlongPath;

        void MoveAlongPath();
        glm::vec2 ConvertToWorldCoordinates(const glm::vec2& relativePos);
    };
}
