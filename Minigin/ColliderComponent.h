#pragma once
#include "BaseComponent.h"
#include <glm/glm.hpp>

namespace dae
{
    class ColliderComponent : public BaseComponent
    {
    public:
        ColliderComponent(GameObject* gameObject, const glm::vec2& size, bool isTrigger = false);
        ~ColliderComponent() = default;

        void SetSize(const glm::vec2& size);
        const glm::vec2& GetSize() const;
        bool IsTrigger() const;

        bool IsCollidingWith(const ColliderComponent* other) const;

    private:
        glm::vec2 m_Size;
        bool m_IsTrigger;
    };
}
