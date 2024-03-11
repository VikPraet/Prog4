#pragma once
#include <iostream>

class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};

class Jump : public Command
{
public:
	void Execute() override { std::cout << "Jump\n"; }
};


//class GameActorCommand : public Command
//{
//	GameActor* m_actor;
//protected:
//	GameActor* GetGameActor() const { return m_actor; }
//public:
//	GameActorCommand(GameActor* actor);
//	virtual ~GameActorCommand();
//};
//
//class Fire : public GameActorCommand
//{
//public:
//	void Execute() override
//	{
//		GetGameActor()->Fire();
//		// additional code is possible too, of course
//	}
//};