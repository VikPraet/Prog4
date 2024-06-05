#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"

namespace dae
{
    class TransformComponent;
    class ColliderComponent;

    class ColliderRenderComponent : public BaseComponent
    {
    public:
        ColliderRenderComponent(GameObject* gameObject, const glm::vec4& color = glm::vec4(0.2f, 0.85f, 0.3f, 1.f));
        ~ColliderRenderComponent() = default;

        void Update() override;
        void Render() const override;

        void SetColor(const glm::vec4& color) { m_Color = color; }
        const glm::vec4& GetColor() const { return m_Color; }

    private:
        glm::vec4 m_Color;
        TransformComponent* m_TransformComponent;
        ColliderComponent* m_ColliderComponent;
    };
}
