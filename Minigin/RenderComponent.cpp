#include "RenderComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TransformComponent.h"

RenderComponent::RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject)
	: BaseComponent(gameObject)
{
}

RenderComponent::RenderComponent(const std::shared_ptr<dae::GameObject>& gameObject, const std::shared_ptr<dae::Texture2D>& texture)
	: BaseComponent(gameObject), m_Texture(texture)
{
}

void RenderComponent::Update()
{
    if(m_TransformComponent == nullptr) m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();
}

void RenderComponent::Render() const
{
	if (m_Texture && m_TransformComponent)
	{
		const auto& pos = m_TransformComponent->GetPosition();
		const float rotation = m_TransformComponent->GetRotation();
		const auto& scale = m_TransformComponent->GetScale();

		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, rotation, scale.x, scale.y);
	}
}

void RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::SetTexture(const std::shared_ptr<dae::Texture2D>& texture)
{
    if(m_Texture == texture) return;
	m_Texture = texture;
}
