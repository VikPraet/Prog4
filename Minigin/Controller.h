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
	void BindThumbCommand(int thumb, std::unique_ptr<dae::Command> command);

	std::pair<float, float> GetThumbValue(int thumb) const;

	int GetIndex() const;

private:
	class ControllerImpl;
	ControllerImpl* m_ControllerImpl;
};
