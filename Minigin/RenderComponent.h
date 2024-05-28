#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

#include <string>


namespace dae
{
    class Texture2D;
    class TransformComponent;

    class RenderComponent final : public BaseComponent
    {
    public:
        RenderComponent(dae::GameObject* gameObject);
        RenderComponent(dae::GameObject* gameObject, std::unique_ptr<dae::Texture2D> texture);
        ~RenderComponent() = default;

        void Update() override;
        void Render() const override;
        void SetTexture(const std::string& filename);

        void SetTexture(std::unique_ptr<dae::Texture2D> texture);

    private:
        std::unique_ptr<dae::Texture2D> m_Texture;
        TransformComponent* m_TransformComponent;
    };
}
