#pragma once
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

    class GameObject final
    {
    public:
        Event<BaseComponent*> OnComponentAdded;

        void FixedUpdate();
        void Update();
        void LateUpdate();
        void Render() const;

        std::string GetTag() const { return m_Tag; }
        void SetTag(const std::string& tag) { m_Tag = tag; }

        void SetActive(bool active) { m_IsActive = active; }
        bool GetActive() const { return m_IsActive; }

        void Destroy();
        bool IsMarkedForDestroy() const { return m_MarkedForDestroy; }

        void DestroyComponents();


        // Add a component to the GameObject
        int AddComponent(std::unique_ptr<BaseComponent> component)
        {
            m_Components.emplace_back(std::move(component));
            auto* newComponent = m_Components.back().get();
            OnComponentAdded.Invoke(newComponent);
            return static_cast<int>(m_Components.size() - 1);
        }

        // Add a component to the GameObject
        template <typename T, typename... Args>
        int AddComponent(Args&&... args)
        {
            m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
            auto* newComponent = m_Components.back().get();
            OnComponentAdded.Invoke(newComponent);
            return static_cast<int>(m_Components.size() - 1);
        }

        // Remove a component from the GameObject
        template <typename T>
        void RemoveComponent()
        {
            for (auto& component : m_Components)
            {
                T* castComponent = dynamic_cast<T*>(component.get());
                if (castComponent != nullptr)
                {
                    castComponent->Destroy();
                }
            }
        }

        void RemoveComponent(BaseComponent* component)
        {
            component->Destroy();
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

        BaseComponent* GetComponentAtIndex(int idx) const
        {
            return m_Components[idx].get();
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

        std::string m_Tag{"untagged"};

        bool m_IsActive{ true };
        bool m_MarkedForDestroy{ false };

        std::vector<std::unique_ptr<BaseComponent>> m_Components;
        GameObject* m_Parent{ nullptr };
        std::unordered_set<GameObject*> m_Children;
    };
}
