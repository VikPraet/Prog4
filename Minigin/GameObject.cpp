#include "GameObject.h"
#include "ResourceManager.h"

namespace dae
{
    void GameObject::Update()
    {
        // Update all components
        for (const auto& component : m_Components)
        {
            component->Update();
        }
    }

    void GameObject::Render() const
    {
        // Render all components
        for (const auto& component : m_Components)
        {
            component->Render();
        }
    }

    void GameObject::SetParent(const std::shared_ptr<GameObject>& parent)
    {
        // Check if the new parent is valid (not itself or one of its children)
        if (parent == nullptr) return;
        if (parent == std::shared_ptr<GameObject>(this)) return;

        // Remove itself from the previous parent

        // Set the given parent on itself

        // Add itself as a child to the given parent

        // Update position, rotation and scale
    }
}
