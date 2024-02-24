#include "RenderComponent.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

RenderComponent::RenderComponent(const std::shared_ptr<dae::Texture2D>& texture)
	: Component(), m_texture(texture)
{
}

void RenderComponent::Render() const
{
    if (m_texture != nullptr)
    {
        const auto& pos = m_transform.GetPosition();
        dae::Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
    }
}

void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}
