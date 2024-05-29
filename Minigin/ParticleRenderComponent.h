#pragma once
#include "BaseComponent.h"
#include "ParticleSystemComponent.h"

namespace dae
{
    struct Particle;

    class ParticleRenderComponent : public BaseComponent
    {
    public:
        ParticleRenderComponent(GameObject* gameObject, ParticleSystemComponent* particleSystem);
        ~ParticleRenderComponent() = default;

        void Render() const override;
        void SetParticleSystem(ParticleSystemComponent* particleSystem) { m_ParticleSystem = particleSystem; }

    private:
        ParticleSystemComponent* m_ParticleSystem;
    };
}
