#include "ParticleRenderComponent.h"
#include "Renderer.h"

namespace dae
{
    ParticleRenderComponent::ParticleRenderComponent(GameObject* gameObject, ParticleSystemComponent* particleSystem)
        : BaseComponent(gameObject), m_ParticleSystem(particleSystem)
    {
    }

    void ParticleRenderComponent::Render() const
    {
        if (!m_ParticleSystem) return;

        const auto& particles = m_ParticleSystem->GetParticles();
        const auto renderer = Renderer::GetInstance().GetSDLRenderer();

        for (const auto& particle : particles)
        {
            SDL_SetRenderDrawColor(renderer, particle.r, particle.g, particle.b, particle.a);

            SDL_Rect rect;
            rect.x = static_cast<int>(particle.x - particle.size / 2);
            rect.y = static_cast<int>(particle.y - particle.size / 2);
            rect.w = static_cast<int>(particle.size);
            rect.h = static_cast<int>(particle.size);

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
