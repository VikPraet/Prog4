#include <map>
#include <Windows.h>
#include <Xinput.h>

#include "Controller.h"
#include "InputManager.h"

class Controller::ControllerImpl
{
public:
	ControllerImpl(int controllerIndex)
		: m_ControllerIndex{ controllerIndex }
	{
		ZeroMemory(&m_PreviousGamepadState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentGamepadState, sizeof(XINPUT_STATE));
	}

	void ProcessInput()
	{
		CopyMemory(&m_PreviousGamepadState, &m_CurrentGamepadState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentGamepadState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentGamepadState);

		// Process amepad commands
		for (const auto& [buttonCode, commandPair] : m_GamepadCommandMap) {
			const bool isPressed = m_CurrentGamepadState.Gamepad.wButtons & buttonCode;
			const bool wasPressed = m_PreviousGamepadState.Gamepad.wButtons & buttonCode;

			switch (commandPair.second) {
			case dae::InputActionType::OnRelease:
				if (!isPressed && wasPressed) {
					commandPair.first->Execute();
				}
				break;

			case dae::InputActionType::OnPressed:
				if (isPressed && !wasPressed) {
					commandPair.first->Execute();
				}
				break;

			case dae::InputActionType::Continuous:
				if (isPressed) {
					commandPair.first->Execute();
				}
				break;
			}
		}
	}

	void BindCommand(int button, std::unique_ptr<Command> command, dae::InputActionType actionType = dae::InputActionType::OnPressed) {
		m_GamepadCommandMap[button] = std::make_pair(std::move(command), actionType);
	}

	int GetIndex() const { return m_ControllerIndex; }

private:
	XINPUT_STATE m_CurrentGamepadState{};
	XINPUT_STATE m_PreviousGamepadState{};
	int m_ControllerIndex{};

	std::map<int, std::pair<std::unique_ptr<Command>, dae::InputActionType>> m_GamepadCommandMap;
};



Controller::Controller(int controllerIndex)
{
	m_ControllerImpl = new ControllerImpl(controllerIndex);
}

Controller::~Controller()
{
	delete m_ControllerImpl;
	m_ControllerImpl = nullptr;
}

void Controller::ProcessInput()
{
	m_ControllerImpl->ProcessInput();
}

void Controller::BindCommand(dae::GamepadButton button, std::unique_ptr<Command> command, dae::InputActionType actionType)
{
	m_ControllerImpl->BindCommand(button.button, std::move(command), actionType);
}

int Controller::GetIndex() const
{
	return m_ControllerImpl->GetIndex();
}
