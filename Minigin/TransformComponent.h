#pragma once
#include "BaseComponent.h"
#include <glm/glm.hpp>

class TransformComponent final: public BaseComponent
{
public:
    TransformComponent(const std::shared_ptr<dae::GameObject>& gameObject);

    const glm::vec3& GetPosition() const { return m_position; }
    void SetPosition(float x, float y, float z = 0);

    float GetRotation() const { return m_rotation; }
    void SetRotation(float angle);

    const glm::vec3& GetScale() const { return m_scale; }
    void SetScale(float scaleX, float scaleY, float scaleZ = 1);

private:
    glm::vec3 m_position;
    float m_rotation;
    glm::vec3 m_scale;
};
