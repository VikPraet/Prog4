#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

namespace dae
{
    void GameObject::Update()
    {
        // Update all components
        for (const auto& component : m_components)
        {
            component->Update();
        }
    }

    void GameObject::Render() const
    {
        // Render all components
        for (const auto& component : m_components)
        {
            component->Render();
        }
    }

    void GameObject::SetPosition(float x, float y)
    {
        m_transform.SetPosition(x, y, 0.0f);
        for (const auto& component : m_components)
        {
            component->SetPosition(x, y, 0.0f);
        }
    }
}
