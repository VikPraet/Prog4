#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"
#include "Event.h"
#include <unordered_set>

namespace dae
{
    class TransformComponent;

    class ColliderComponent : public BaseComponent
    {
    public:
        ColliderComponent(GameObject* gameObject, const glm::vec2& size);

        void Update() override;
        void FixedUpdate() override;

        const glm::vec2& GetSize() const { return m_Size; }

        bool IsOverlappingWith(const ColliderComponent* other) const;

        Event<GameObject*, GameObject*> OnTriggerEnterEvent;
        Event<GameObject*, GameObject*> OnTriggerExitEvent;

    private:
        glm::vec2 m_Size;
        TransformComponent* m_TransformComponent;
        std::unordered_set<GameObject*> m_CurrentOverlaps;

        void CheckOverlaps();
    };
}
