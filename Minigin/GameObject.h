#pragma once
#include <memory>
#include <vector>

#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
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
            m_components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        // Remove a component from the GameObject
        template <typename T>
        void RemoveComponent()
        {
            m_components.erase(
                std::remove_if(
                    m_components.begin(),
                    m_components.end(),
                    [](const auto& component) {
                        return dynamic_cast<T*>(component.get()) != nullptr;
                    }),
                m_components.end());
        }

        // Get a component from the GameObject
        template <typename T>
        T* GetComponent()
        {
            for (const auto& component : m_components)
            {
                if (auto castedComponent = dynamic_cast<T*>(component.get()))
                    return castedComponent;
            }
            return nullptr;
        }

        GameObject() = default;
        ~GameObject() = default;
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

    private:
        Transform m_transform{};
        std::vector<std::unique_ptr<Component>> m_components;
    };
}
