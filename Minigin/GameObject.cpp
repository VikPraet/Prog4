#include "GameObject.h"
#include "ResourceManager.h"
#include "TransformComponent.h"

namespace dae
{
	void GameObject::FixedUpdate()
	{
        // Update all components
        for (const auto& component : m_Components)
        {
            if (component->GetActive())
                component->FixedUpdate();
        }
	}

    void GameObject::Update()
    {
        // Update all components
        for (const auto& component : m_Components)
        {
            if(component->GetActive())
                component->Update();
        }
    }

    void GameObject::LateUpdate()
    {
        // Update all components
        for (const auto& component : m_Components)
        {
            if (component->GetActive())
                component->LateUpdate();
        }
    }

    void GameObject::Render() const
    {
        // Render all components
        for (const auto& component : m_Components)
        {
            if (component->GetActive())
                component->Render();
        }
    }

    void GameObject::Destroy()
    {
        m_IsActive = false;
        m_MarkedForDestroy = true;

        for (auto& child : m_Children)
        {
            if(child) child->Destroy();
        }
    }

    void GameObject::DestroyComponents()
    {
        std::erase_if(m_Components,
            [&](const std::unique_ptr<BaseComponent>& component)
            {
                return component->IsMarkedForDestroy();
            }
        );
    }

    void GameObject::SetParent(GameObject* newParent, bool keepWorldPosition)
    {
        // Validate the new parent: it should not be the same as this object, or a child of this object
        if (newParent == this || IsChild(newParent)) return;

        // If this object already has a parent, remove this object from its parent's children
        if (m_Parent)
        {
            m_Parent->m_Children.erase(this);
            UpdateLocalPositionRelativeToParent(keepWorldPosition, true);
        }

        // Set the new parent
        m_Parent = newParent;

        // If the new parent is valid, add this object to the new parent's children
        if (m_Parent)
        {
            m_Parent->m_Children.insert(this);
            UpdateLocalPositionRelativeToParent(keepWorldPosition, false);
        }

        // Mark the transform component dirty
        GetComponent<TransformComponent>()->SetPositionDirty();
    }

    bool GameObject::IsChild(GameObject* gameObject) const
    {
        if (m_Children.contains(gameObject))
            return true;

        for (const auto* child : m_Children)
        {
            if (child->IsChild(gameObject))
                return true;
        }

        return false;
    }

    void GameObject::UpdateLocalPositionRelativeToParent(bool keepWorldPosition, bool isRemoving)
    {
        if (!keepWorldPosition || !m_Parent) return;

        const auto transformComponent = GetComponent<TransformComponent>();
        const auto parentWorldPosition = m_Parent->GetComponent<TransformComponent>()->GetWorldPosition();

        if (transformComponent == nullptr) return;

        const auto localPosition = transformComponent->GetLocalPosition();

        // Update local position based on parent's world position
        transformComponent->SetLocalPosition(
            isRemoving ? (localPosition + parentWorldPosition) : (localPosition - parentWorldPosition)
        );
    }
}
