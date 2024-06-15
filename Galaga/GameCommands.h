#pragma once
#include "Command.h"
#include "PlayerAttackBehavior.h"
#include "PlayerMovementBehavior.h"
#include "ServiceLocator.h"

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
                ServiceLocator::GetService<ISoundService>()->SetSoundVolume(m_SoundVolume);
                ServiceLocator::GetService<ISoundService>()->SetMusicVolume(m_MusicVolume);
                m_IsMuted = false;
            }
            else
            {
                m_SoundVolume = ServiceLocator::GetService<ISoundService>()->GetSoundVolume();
                m_MusicVolume = ServiceLocator::GetService<ISoundService>()->GetMusicVolume();
                ServiceLocator::GetService<ISoundService>()->SetSoundVolume(0);
                ServiceLocator::GetService<ISoundService>()->SetMusicVolume(0);
                m_IsMuted = true;
            }
        }

    private:
        bool m_IsMuted{ false };
        int m_SoundVolume{};
        int m_MusicVolume{};
    };
}
