#pragma once
#include "BaseComponent.h"
#include "Singleton.h"
#include "GameObject.h"
#include "SceneManager.h"

namespace dae
{
    template <typename T>
    class SingletonComponent : public BaseComponent, public Singleton<T>
    {
    public:
        // Constructor with default argument to handle nullptr
        SingletonComponent(GameObject* gameObject = nullptr)
            : BaseComponent(gameObject ? gameObject : createAndAddComponent())
        {
        }

        ~SingletonComponent() = default;

    private:
        static GameObject* createAndAddComponent()
        {
            auto gameObject = std::make_unique<GameObject>();
            GameObject* gameObjectPtr = gameObject.get();
            gameObject->AddComponent<T>(gameObjectPtr);
            SceneManager::GetInstance().GetActiveScene()->Add(std::move(gameObject));
            return gameObjectPtr;
        }
    };
}
