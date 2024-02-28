#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

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
}
