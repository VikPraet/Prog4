#pragma once
#include "Component.h"

#include <string>
#include <SDL_rect.h>

namespace dae
{
	class Texture2D;
}

class RenderComponent : public Component
{
public:
    RenderComponent() = default;
    RenderComponent(const std::shared_ptr<dae::Texture2D>& texture);
    virtual ~RenderComponent() = default;

    void Render() const override;
    void SetTexture(const std::string& filename);

protected:
    std::shared_ptr<dae::Texture2D> m_texture;
};
