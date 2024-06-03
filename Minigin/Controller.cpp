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

		// Process gamepad commands
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

		for (const auto& [thumbCode, commandPair] : m_ThumbCommandMap) {
			auto [x, y] = GetThumbValue(thumbCode);
			if (commandPair.second == dae::InputActionType::Continuous) {
				dynamic_cast<dae::GameObjectStickCommand*>(commandPair.first.get())->Execute(x, y);
			}
		}
	}

	void BindThumbCommand(int thumb, std::unique_ptr<dae::Command> command, dae::InputActionType actionType) {
		m_ThumbCommandMap[thumb] = std::make_pair(std::move(command), actionType);
	}

	void BindCommand(int button, std::unique_ptr<dae::Command> command, dae::InputActionType actionType = dae::InputActionType::OnPressed) {
		m_GamepadCommandMap[button] = std::make_pair(std::move(command), actionType);
	}

	int GetIndex() const { return m_ControllerIndex; }

	float ApplyDeadzoneAndRemap(SHORT value, SHORT deadzone) const {
		if (std::abs(value) < deadzone) {
			return 0.0f;
		}

		const float sign = static_cast<float>(value / std::abs(value));
		const float adjustedValue = (std::abs(value) - deadzone) / (32767.0f - deadzone);
		return adjustedValue * sign;
	}

	std::pair<float, float> GetThumbValue(int thumb) const {
		const SHORT deadzone = 8000;
		SHORT xValue = 0, yValue = 0;
		if (thumb == XINPUT_GAMEPAD_LEFT_THUMB) {
			xValue = m_CurrentGamepadState.Gamepad.sThumbLX;
			yValue = m_CurrentGamepadState.Gamepad.sThumbLY;
		}
		else if (thumb == XINPUT_GAMEPAD_RIGHT_THUMB) {
			xValue = m_CurrentGamepadState.Gamepad.sThumbRX;
			yValue = m_CurrentGamepadState.Gamepad.sThumbRY;
		}

		float remappedX = ApplyDeadzoneAndRemap(xValue, deadzone);
		float remappedY = ApplyDeadzoneAndRemap(yValue, deadzone);

		return { remappedX, remappedY };
	}


private:
	XINPUT_STATE m_CurrentGamepadState{};
	XINPUT_STATE m_PreviousGamepadState{};
	int m_ControllerIndex{};

	std::map<int, std::pair<std::unique_ptr<dae::Command>, dae::InputActionType>> m_GamepadCommandMap;
	std::map<int, std::pair<std::unique_ptr<dae::Command>, dae::InputActionType>> m_ThumbCommandMap;
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

void Controller::BindCommand(dae::GamepadButton button, std::unique_ptr<dae::Command> command, dae::InputActionType actionType)
{
	m_ControllerImpl->BindCommand(button.button, std::move(command), actionType);
}

void Controller::BindThumbCommand(int thumb, std::unique_ptr<dae::Command> command, dae::InputActionType actionType)
{
	m_ControllerImpl->BindThumbCommand(thumb, std::move(command), actionType);
}

std::pair<float, float> Controller::GetThumbValue(int thumb) const
{
	return m_ControllerImpl->GetThumbValue(thumb);
}

int Controller::GetIndex() const
{
	return m_ControllerImpl->GetIndex();
}
