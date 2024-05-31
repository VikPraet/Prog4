#pragma once
#include "Event.h"
#include "GameObject.h"

namespace dae
{
    class CollisionListener final : public EventListener
    {
    public:
        CollisionListener(GameObject* gameObject);
        void OnCollision(GameObject* self, GameObject* other);

    private:
        GameObject* m_GameObject;
    };

    class CollisionEvent final : public Event<GameObject*, GameObject*>
    {
    public:
        using Event::Event;
    };
}
