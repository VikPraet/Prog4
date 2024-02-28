#pragma once
#include "BaseComponent.h"

#include <string>

class TransformComponent;

namespace dae
{
	class Texture2D;
}

class RenderComponent final: public BaseComponent
{
public:
    RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject);
    RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::Texture2D>& texture);
    ~RenderComponent() = default;

    void Update() override;
    void Render() const override;
    void SetTexture(const std::string& filename);

    std::shared_ptr<dae::Texture2D> GetTexture() const { return m_Texture; }
    void SetTexture(const std::shared_ptr<dae::Texture2D>& texture);

private:
    std::shared_ptr<dae::Texture2D> m_Texture;
    TransformComponent* m_TransformComponent;
};
