#pragma once
#include <memory>
#include <unordered_set>
#include <vector>

#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

    class GameObject final
    {
    public:
        void FixedUpdate();
        void Update();
        void LateUpdate();
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

        GameObject* GetParent() const { return m_Parent; }
        void SetParent(GameObject* newParent, bool keepWorldPosition = true);

        int GetChildCount() const { return static_cast<int>(m_Children.size()); }
        const std::unordered_set<GameObject*>& GetChildren() { return m_Children; }
        bool IsChild(GameObject* gameObject) const;


        GameObject() = default;
        ~GameObject() = default;
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

    private:
        void UpdateLocalPositionRelativeToParent(bool keepWorldPosition, bool isRemoving);

        std::vector<std::unique_ptr<BaseComponent>> m_Components;
        GameObject* m_Parent;
        std::unordered_set<GameObject*> m_Children;
    };
}
