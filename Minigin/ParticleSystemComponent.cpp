#include "ParticleSystemComponent.h"
#include "GameTime.h"

namespace dae
{
    ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject)
        : BaseComponent(gameObject)
    {
    }

    void ParticleSystemComponent::Update()
    {
        const float deltaTime = GameTime::GetInstance().GetDeltaTime();

        for (auto& particle : particles)
        {
            particle.x += particle.vx * deltaTime;
            particle.y += particle.vy * deltaTime;
            particle.lifespan -= deltaTime;

            // Fade out particles
            particle.a = static_cast<Uint8>(255 * (particle.lifespan / 1.0f));
        }

        // Remove dead particles
        std::erase_if(particles, [](const Particle& p) { return p.lifespan <= 0; });

        // Emit new particles if in continuous mode
        if (emissionMode == EmissionMode::Continuous && static_cast<int>(particles.size()) < targetNumberOfParticles)
        {
            timeSinceLastEmission += deltaTime;
            while (timeSinceLastEmission > 1.0f / emissionRate)
            {
                Emit(static_cast<int>(spawnStartX), static_cast<int>(spawnStartY)); // Emit from defined area
                timeSinceLastEmission -= 1.0f / emissionRate;
            }
        }
    }

    void ParticleSystemComponent::Emit(int x, int y)
    {
        if (emissionMode == EmissionMode::Once)
        {
            for (int i = 0; i < targetNumberOfParticles; ++i)
            {
                CreateParticle(x, y);
            }
        }
        else
        {
            CreateParticle(x, y);
        }
    }

    void ParticleSystemComponent::SetSeedColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool randomize, Uint8 rRange, Uint8 gRange, Uint8 bRange, Uint8 aRange)
    {
        seedR = r;
        seedG = g;
        seedB = b;
        seedA = a;
        randomizeColor = randomize;
        seedRRange = rRange;
        seedGRange = gRange;
        seedBRange = bRange;
        seedARange = aRange;
    }

    void ParticleSystemComponent::SetSeedVelocity(float vx, float vy, bool randomize, float vxRange, float vyRange)
    {
        seedVx = vx;
        seedVy = vy;
        randomizeVelocity = randomize;
        seedVxRange = vxRange;
        seedVyRange = vyRange;
    }

    void ParticleSystemComponent::SetSeedLifespan(float lifespan, bool randomize, float lifespanRange)
    {
        seedLifespan = lifespan;
        randomizeLifespan = randomize;
        seedLifespanRange = lifespanRange;
    }

    void ParticleSystemComponent::SetSeedSize(float size, bool randomize, float sizeRange)
    {
        seedSize = size;
        randomizeSize = randomize;
        seedSizeRange = sizeRange;
    }

    void ParticleSystemComponent::SetSpawnArea(float startX, float startY, bool randomize, float width, float height)
    {
        spawnStartX = startX;
        spawnStartY = startY;
        randomizeSpawnArea = randomize;
        spawnWidth = width;
        spawnHeight = height;
    }

    void ParticleSystemComponent::SetTargetNumberOfParticles(int target)
    {
        targetNumberOfParticles = target;
    }

    void ParticleSystemComponent::SetEmissionRate(float rate)
    {
        emissionRate = rate;
    }

    void ParticleSystemComponent::SetEmissionMode(EmissionMode mode)
    {
        emissionMode = mode;
    }

    void ParticleSystemComponent::CreateParticle(int x, int y)
    {
        Particle particle;
        float spawnX = static_cast<float>(x);
        float spawnY = static_cast<float>(y);

        // Calculate the spawn position within the defined area
        if (randomizeSpawnArea) {
            spawnX += Randomize(0, spawnWidth);
            spawnY += Randomize(0, spawnHeight);
        }

        particle.x = spawnX;
        particle.y = spawnY;

        // Use seed velocity with some random variation
        particle.vx = randomizeVelocity ? Randomize(seedVx, seedVxRange) : seedVx;
        particle.vy = randomizeVelocity ? Randomize(seedVy, seedVyRange) : seedVy;

        // Use seed color with some random variation
        particle.r = randomizeColor ? Randomize(seedR, seedRRange) : seedR;
        particle.g = randomizeColor ? Randomize(seedG, seedGRange) : seedG;
        particle.b = randomizeColor ? Randomize(seedB, seedBRange) : seedB;
        particle.a = randomizeColor ? Randomize(seedA, seedARange) : seedA;

        // Use seed lifespan with some random variation
        particle.lifespan = randomizeLifespan ? Randomize(seedLifespan, seedLifespanRange) : seedLifespan;

        // Use seed size with some random variation
        particle.size = randomizeSize ? Randomize(seedSize, seedSizeRange) : seedSize;

        particles.push_back(particle);
    }

    float ParticleSystemComponent::Randomize(float value, float range)
    {
        return value + distribution(generator) * range - range / 2.0f;
    }

    Uint8 ParticleSystemComponent::Randomize(Uint8 value, Uint8 range)
    {
        return value + static_cast<Uint8>(distribution(generator) * range) - range / 2;
    }
}
