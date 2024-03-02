#pragma once
#include "BaseComponent.h"
#include <glm/glm.hpp>

class TransformComponent final: public BaseComponent
{
public:
    TransformComponent(const std::shared_ptr<dae::GameObject>& gameObject);
    ~TransformComponent() = default;

    const glm::vec3& GetWorldPosition();
    void SetWorldPosition(float x, float y, float z = 0);
    void SetWorldPosition(glm::vec3 worldPosition);

    const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
    void SetLocalPosition(float x, float y, float z = 0);
    void SetLocalPosition(glm::vec3 localPosition);

    float GetRotation() const { return m_rotation; }
    void SetRotation(float angle);

    const glm::vec3& GetScale() const { return m_scale; }
    void SetScale(float scaleX, float scaleY, float scaleZ = 1);
    void SetScale(float scale);

    void SetPositionDirty();
private:
    glm::vec3 m_WorldPosition;
    glm::vec3 m_LocalPosition;
    float m_rotation;
    glm::vec3 m_scale;

    bool m_PositionDirty{true};

    void UpdateWorldPosition();
};