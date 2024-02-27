#include "RenderComponent.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

RenderComponent::RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject)
	: Component(gameObject)
{
}

RenderComponent::RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::Texture2D>& texture)
	: Component(gameObject), m_texture(texture)
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

void RenderComponent::SetTexture(const std::shared_ptr<dae::Texture2D>& texture)
{
    if(m_texture == texture) return;
	m_texture = texture;
}
