#include "ColliderRenderComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "Renderer.h"

dae::ColliderRenderComponent::ColliderRenderComponent(GameObject* gameObject, const glm::vec4& color)
    : BaseComponent(gameObject), m_Color(color), m_TransformComponent(nullptr), m_ColliderComponent(nullptr)
{
}

void dae::ColliderRenderComponent::Update()
{
    if (!m_TransformComponent)
        m_TransformComponent = GetGameObject()->GetComponent<TransformComponent>();
    if (!m_ColliderComponent)
        m_ColliderComponent = GetGameObject()->GetComponent<ColliderComponent>();
}

void dae::ColliderRenderComponent::Render() const
{
    if (!m_TransformComponent || !m_ColliderComponent)
        return;

    const auto pos = glm::vec2(m_TransformComponent->GetWorldPosition());
    const auto size = m_ColliderComponent->GetSize();
    const auto renderer = Renderer::GetInstance().GetSDLRenderer();

    SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(m_Color.r * 255), static_cast<Uint8>(m_Color.g * 255), static_cast<Uint8>(m_Color.b * 255), static_cast<Uint8>(m_Color.a * 255));

    SDL_Rect rect;
    rect.x = static_cast<int>(pos.x - size.x / 2);
    rect.y = static_cast<int>(pos.y - size.y / 2);
    rect.w = static_cast<int>(size.x);
    rect.h = static_cast<int>(size.y);

    SDL_RenderDrawRect(renderer, &rect);
}

