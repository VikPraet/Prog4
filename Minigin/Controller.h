#pragma once
#include <memory>

#include "Command.h"

namespace dae
{
	enum class InputActionType;
	struct GamepadButton;
}

class Controller final
{
public:
	Controller(int controllerIndex);
	~Controller();

	void ProcessInput();

	void BindCommand(dae::GamepadButton button, std::unique_ptr<dae::Command> command, dae::InputActionType actionType);

	int GetIndex() const;

private:
	class ControllerImpl;
	ControllerImpl* m_ControllerImpl;
};
