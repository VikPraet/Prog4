#pragma once
#include <memory>
#include <vector>

#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

    class GameObject final
    {
    public:
        void Update();
        void Render() const;

        // Add a component to the GameObject
        template <typename T, typename... Args>
        void AddComponent(Args&&... args)
        {
            m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        // Remove a component from the GameObject
        template <typename T>
        void RemoveComponent()
        {
            m_Components.erase(
                std::remove_if(
                    m_Components.begin(),
                    m_Components.end(),
                    [](const auto& component) {
                        return dynamic_cast<T*>(component.get()) != nullptr;
                    }),
                m_Components.end());
        }

        // Get a component from the GameObject
        template <typename T>
        T* GetComponent()
        {
            for (const auto& component : m_Components)
            {
                if (auto castedComponent = dynamic_cast<T*>(component.get()))
                    return castedComponent;
            }
            return nullptr;
        }

        // Check if gameObject has this type of component
        template <typename T>
        bool HasComponent() const
        {
            for (const auto& component : m_Components)
            {
                if (dynamic_cast<T*>(component.get()) != nullptr)
                    return true;
            }
            return false;
        }

        std::shared_ptr<GameObject> GetParent() const { return m_Parent; }
        void SetParent(const std::shared_ptr<GameObject>& parent);

        int GetChildCount() const { return static_cast<int>(m_Children.size()); }
        std::shared_ptr<GameObject> GetChildAt(const int index) { return m_Children[index]; }

        // todo;

        //Can SetParent use AddChild to do its job ?
        //Can AddChild use SetParent to do its job ?
        //No – stack overflow would happen

        // Do we really need AddChild / RemoveChild ?
        // No, being able to set the parent on a GameObject is enough.
        // Set the parent to nullptr to remove the child from its parent.
        void AddChild(const std::shared_ptr<GameObject>& child);
        void RemoveChild(const std::shared_ptr<GameObject>& child);

        GameObject() = default;
        ~GameObject() = default;
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

    private:
        std::vector<std::unique_ptr<BaseComponent>> m_Components;

        std::shared_ptr<GameObject> m_Parent;
        std::vector<std::shared_ptr<GameObject>> m_Children;
    };
}
