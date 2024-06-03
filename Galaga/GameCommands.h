#pragma once
#include "Command.h"
#include "GameTime.h"
#include "PlayerAttackBehavior.h"
#include "PlayerMovementBehavior.h"

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
}
