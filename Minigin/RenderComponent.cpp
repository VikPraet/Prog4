#include "RenderComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"

RenderComponent::RenderComponent(dae::GameObject* gameObject)
	: BaseComponent(gameObject)
{
}

RenderComponent::RenderComponent(dae::GameObject* gameObject, std::unique_ptr<dae::Texture2D> texture)
	: BaseComponent(gameObject), m_Texture(std::move(texture))
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
		const auto& pos = m_TransformComponent->GetWorldPosition();
		const float rotation = m_TransformComponent->GetRotation();
		const auto& scale = m_TransformComponent->GetScale();

		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, rotation, scale.x, scale.y);
	}
}

void RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::SetTexture(std::unique_ptr<dae::Texture2D> texture)
{
    if(m_Texture == texture) return;
	m_Texture = std::move(texture);
}
