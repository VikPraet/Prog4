#pragma once
#include <map>
#include <memory>
#include <SDL_keycode.h>
#include <Windows.h>
#include <Xinput.h>

#include "Command.h"
#include "Singleton.h"

namespace dae
{
	enum class InputActionType {
		OnPressed,
		OnRelease,
		Continuous
	};

	struct GamepadButton {
		int button;
		explicit GamepadButton(int buttonCode) : button(buttonCode) {}
	};

	struct KeyboardKey {
		SDL_Keycode key;
		explicit KeyboardKey(SDL_Keycode keyCode) : key(keyCode) {}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void BindCommand(GamepadButton button, std::unique_ptr<Command> command, InputActionType actionType = InputActionType::OnPressed) {
			m_GamepadCommandMap[button.button] = std::make_pair(std::move(command), actionType);
		}

		void BindCommand(KeyboardKey key, std::unique_ptr<Command> command, InputActionType actionType = InputActionType::OnPressed) {
			m_KeyboardCommandMap[key.key] = std::make_pair(std::move(command), actionType);
		}

	private:
		// Gamepad
		XINPUT_STATE m_CurrentGamepadState{};
		XINPUT_STATE m_PreviousGamepadState{};
		int m_ControllerIndex{};

		std::map<int, std::pair<std::unique_ptr<Command>, InputActionType>> m_GamepadCommandMap;

		// Keyboard
		std::map<SDL_Keycode, bool> m_CurrentKeyStateMap;
		std::map<SDL_Keycode, bool> m_PreviousKeyStateMap;

		std::map<int, std::pair<std::unique_ptr<Command>, InputActionType>> m_KeyboardCommandMap;
	};
}
