#pragma once
#include "BaseComponent.h"
#include <vector>
#include <SDL.h>
#include <random>

namespace dae
{
    struct Particle {
        float x, y;           // Position
        float vx, vy;         // Velocity
        Uint8 r, g, b, a;     // Color
        float lifespan;       // Lifespan in seconds
        float size;           // Size of the particle
    };

    class ParticleSystemComponent : public BaseComponent
    {
    public:
        enum class EmissionMode {
            Once,
            Continuous
        };

        ParticleSystemComponent(GameObject* gameObject);
        ~ParticleSystemComponent() override = default;

        void Update() override;
        void Emit(int x, int y);
        const std::vector<Particle>& GetParticles() const { return particles; }

        // Setters for seed properties
        void SetSeedColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool randomize = false, Uint8 rRange = 0, Uint8 gRange = 0, Uint8 bRange = 0, Uint8 aRange = 0);
        void SetSeedVelocity(float vx, float vy, bool randomize = false, float vxRange = 0.0f, float vyRange = 0.0f);
        void SetSeedLifespan(float lifespan, bool randomize = false, float lifespanRange = 0.0f);
        void SetSeedSize(float size, bool randomize = false, float sizeRange = 0.0f);

        // Setters for spawn area
        void SetSpawnArea(float startX, float startY, bool randomize = false, float width = 0.0f, float height = 0.0f);

        // Set target number of particles and emission rate
        void SetTargetNumberOfParticles(int target);
        void SetEmissionRate(float rate); // Particles per second
        void SetEmissionMode(EmissionMode mode);

    private:
        std::vector<Particle> particles;

        // Seed properties for particles
        Uint8 seedR = 255, seedG = 255, seedB = 255, seedA = 255;
        Uint8 seedRRange = 0, seedGRange = 0, seedBRange = 0, seedARange = 0;
        bool randomizeColor = false;

        float seedVx = 0.0f, seedVy = 0.0f;
        float seedVxRange = 0.0f, seedVyRange = 0.0f;
        bool randomizeVelocity = false;

        float seedLifespan = 10.0f;
        float seedLifespanRange = 0.0f;
        bool randomizeLifespan = false;

        float seedSize = 1.0f;
        float seedSizeRange = 0.0f;
        bool randomizeSize = false;

        // Spawn area properties
        float spawnStartX = 0.0f, spawnStartY = 0.0f;
        float spawnWidth = 0.0f, spawnHeight = 0.0f;
        bool randomizeSpawnArea = false;

        // Emission control properties
        int targetNumberOfParticles = 100;
        float emissionRate = 10.0f; // Particles per second
        float timeSinceLastEmission = 0.0f;
        EmissionMode emissionMode = EmissionMode::Once;

        std::default_random_engine generator{ std::random_device{}() };
        std::uniform_real_distribution<float> distribution{ 0.0f, 1.0f };

        void CreateParticle(int x, int y);
        float Randomize(float value, float range);
        Uint8 Randomize(Uint8 value, Uint8 range);
    };
}
