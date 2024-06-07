#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"
#include <memory>
#include <string>

namespace dae
{
    class Texture2D;
    class TransformComponent;
    class AnimatorComponent;

    class RenderComponent final : public BaseComponent
    {
    public:
        RenderComponent(GameObject* gameObject);
        RenderComponent(GameObject* gameObject, std::unique_ptr<Texture2D> texture);
        ~RenderComponent() = default;

        void Update() override;
        void Render() const override;
        void OnComponentAdded(BaseComponent* component) override;

        void SetTexture(const std::string& filename);
        void SetTexture(std::unique_ptr<Texture2D> texture);
        void SetAnimator(AnimatorComponent* animator);

        Texture2D* GetTexture() const;

    private:
        std::unique_ptr<Texture2D> m_Texture;
        TransformComponent* m_TransformComponent{ nullptr };
        AnimatorComponent* m_AnimatorComponent{ nullptr };
    };
}
