#include "RenderComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "AnimatorComponent.h"

dae::RenderComponent::RenderComponent(GameObject* gameObject)
    : BaseComponent(gameObject), m_Texture(nullptr)
{
}

dae::RenderComponent::RenderComponent(GameObject* gameObject, std::unique_ptr<Texture2D> texture)
    : BaseComponent(gameObject), m_Texture(std::move(texture))
{
}

void dae::RenderComponent::Update()
{
    if (m_TransformComponent == nullptr) m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();
}

void dae::RenderComponent::Render() const
{
    if (m_Texture && m_TransformComponent)
    {
        const auto& pos = m_TransformComponent->GetWorldPosition();
        const float rotation = m_TransformComponent->GetRotation();
        const auto& scale = m_TransformComponent->GetScale();

        if (m_AnimatorComponent)
        {
	        const SDL_Rect srcRect = m_AnimatorComponent->GetCurrentFrameRect();
            Renderer::GetInstance().RenderTexture(*m_Texture, srcRect, pos.x, pos.y, rotation, scale.x, scale.y);
        }
        else
        {
            Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, rotation, scale.x, scale.y);
        }
    }
}

void dae::RenderComponent::OnComponentAdded(BaseComponent* component)
{
    if (const auto animator = dynamic_cast<AnimatorComponent*>(component))
    {
        m_AnimatorComponent = animator;
    }
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
    m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
{
    if (m_Texture == texture) return;
    m_Texture = std::move(texture);
}

void dae::RenderComponent::SetAnimator(AnimatorComponent* animator)
{
    m_AnimatorComponent = animator;
}

dae::Texture2D* dae::RenderComponent::GetTexture() const
{
    return m_Texture.get();
}