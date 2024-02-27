#pragma once
#include "Component.h"

#include <string>

namespace dae
{
	class Texture2D;
}

class RenderComponent final: public Component
{
public:
    RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject);
    RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::Texture2D>& texture);
    virtual ~RenderComponent() = default;

    void Render() const override;
    void SetTexture(const std::string& filename);

    std::shared_ptr<dae::Texture2D> GetTexture() const { return m_texture; }
    void SetTexture(const std::shared_ptr<dae::Texture2D>& texture);

private:
    std::shared_ptr<dae::Texture2D> m_texture;
};
