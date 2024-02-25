#pragma once
#include <memory>
#include <vector>

#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Texture2D;

    class GameObject final
    {
    public:
        void Update();
        void Render() const;

        void SetPosition(float x, float y);

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

        GameObject() = default;
        ~GameObject() = default;
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

    private:
        Transform m_transform{};
        std::vector<std::unique_ptr<Component>> m_Components;
    };
}
