#pragma once

#include "BaseComponent.h"
#include <glm/glm.hpp>

namespace dae
{
    class TransformComponent;

    class ColliderComponent : public BaseComponent
    {
    public:
        ColliderComponent(GameObject* gameObject, const glm::vec2& size);
        ~ColliderComponent() = default;

        void SetSize(const glm::vec2& size);
        const glm::vec2& GetSize() const;

        bool IsCollidingWith(const ColliderComponent* other);

    private:
        glm::vec2 m_Size;
        mutable TransformComponent* m_TransformComponent;
    };
}
