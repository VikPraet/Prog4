#pragma once
#include <iostream>

#include "GameObject.h"
#include "PlayerMovementComponent.h"

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};

class GameActorCommand : public Command
{
public:
	GameActorCommand(const std::shared_ptr<dae::GameObject>& actor)
		: m_Actor(actor)
	{
		m_PlayerMovementComponent = GetGameActor()->GetComponent<PlayerMovementComponent>();
	}

	virtual ~GameActorCommand() = default;

protected:
	std::shared_ptr<dae::GameObject> GetGameActor() const { return std::shared_ptr(m_Actor); }
	PlayerMovementComponent* GetPlayerMovementComponent() const { return m_PlayerMovementComponent; }

private:
	std::weak_ptr<dae::GameObject> m_Actor;
	PlayerMovementComponent* m_PlayerMovementComponent;
};

class MoveLeftCommand final : public GameActorCommand {
public:
	MoveLeftCommand(const std::shared_ptr<dae::GameObject>& actor) : GameActorCommand(actor) {}

	void Execute() override {
		if (const auto PMComp = GetPlayerMovementComponent()) {
			PMComp->MoveLeft();
		}
	}
};

class MoveRightCommand final : public GameActorCommand {
public:
	MoveRightCommand(const std::shared_ptr<dae::GameObject>& actor) : GameActorCommand(actor) {}

	void Execute() override {
		if (const auto PMComp = GetPlayerMovementComponent()) {
			PMComp->MoveRight();
		}
	}
};

class MoveUpCommand final : public GameActorCommand {
public:
	MoveUpCommand(const std::shared_ptr<dae::GameObject>& actor) : GameActorCommand(actor) {}

	void Execute() override {
		if (const auto PMComp = GetPlayerMovementComponent()) {
			PMComp->MoveUp();
		}
	}
};

class MoveDownCommand final : public GameActorCommand {
public:
	MoveDownCommand(const std::shared_ptr<dae::GameObject>& actor) : GameActorCommand(actor) {}

	void Execute() override {
		if (const auto PMComp = GetPlayerMovementComponent()) {
			PMComp->MoveDown();
		}
	}
};