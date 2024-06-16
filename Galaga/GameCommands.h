#pragma once
#include "Command.h"
#include "PlayerAttackBehavior.h"
#include "PlayerMovementBehavior.h"
#include "ServiceLocator.h"
#include "WaveManager.h"

namespace galaga
{
	class MoveCommand final : public dae::GameObjectCommand
    {
    public:
        MoveCommand(dae::GameObject* gameObject, const glm::vec2& direction)
            : GameObjectCommand(gameObject), m_Direction(direction)
        {
            m_PlayerMovement = GetGameObject()->GetComponent<PlayerMovementBehavior>();
        }

        void Execute() override
        {
            if (m_PlayerMovement)
            {
                m_PlayerMovement->Move(m_Direction);
            }
        }

    private:
        glm::vec2 m_Direction;
        PlayerMovementBehavior* m_PlayerMovement;
    };

    class ThumbMoveCommand final : public dae::GameObjectStickCommand {
    public:
        ThumbMoveCommand(dae::GameObject* gameObject)
    		: GameObjectStickCommand(gameObject)
    	{
            m_MovementBehavior = GetGameObject()->GetComponent<PlayerMovementBehavior>();
        }

        void Execute(float x, float y) override {
            if (m_MovementBehavior) {
                m_MovementBehavior->Move(glm::vec2{ x, y });
            }
        }
    private:
        PlayerMovementBehavior* m_MovementBehavior;
    };


    class ShootCommand final : public dae::GameObjectCommand
    {
    public:
        ShootCommand(dae::GameObject* gameObject)
            : GameObjectCommand(gameObject)
        {
            m_AttackBehavior = GetGameObject()->GetComponent<PlayerAttackBehavior>();
        }

        void Execute() override
        {
            if (m_AttackBehavior)
            {
                m_AttackBehavior->Shoot();
            }
        }

    private:
        PlayerAttackBehavior* m_AttackBehavior;
    };

    class MuteCommand final : public dae::Command
    {
    public:
        MuteCommand() = default;

        void Execute() override
        {
            if(m_IsMuted)
            {
	            dae::ServiceLocator::GetService<ISoundService>()->SetSoundVolume(m_SoundVolume);
	            dae::ServiceLocator::GetService<ISoundService>()->SetMusicVolume(m_MusicVolume);
                m_IsMuted = false;
            }
            else
            {
                m_SoundVolume = dae::ServiceLocator::GetService<ISoundService>()->GetSoundVolume();
                m_MusicVolume = dae::ServiceLocator::GetService<ISoundService>()->GetMusicVolume();
                dae::ServiceLocator::GetService<ISoundService>()->SetSoundVolume(0);
                dae::ServiceLocator::GetService<ISoundService>()->SetMusicVolume(0);
                m_IsMuted = true;
            }
        }

    private:
        bool m_IsMuted{ false };
        int m_SoundVolume{};
        int m_MusicVolume{};
    };

    class SkipWaveCommand final : public dae::Command
    {
    public:
        SkipWaveCommand() = default;

        void Execute() override
        {
            const auto& waveManagers = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("WaveManager");
	        const auto& enemies = dae::SceneManager::GetInstance().GetActiveScene()->GetGameObjectsWithTag("enemy");
            for(const auto& enemy : enemies)
            {
                enemy->Destroy();

                if(waveManagers[0])
                {
                    if (const auto& waveManagerComponent = waveManagers[0]->GetComponent<WaveManager>()) 
                        waveManagerComponent->CheckAndStartNextWave();
                }
            }
        }
    };
}
