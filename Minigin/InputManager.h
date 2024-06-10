#pragma once
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <SDL_keycode.h>

#include "Command.h"
#include "Controller.h"
#include "Singleton.h"

namespace dae
{
	enum class InputActionType {
		OnPressed,
		OnRelease,
		Continuous
	};

	struct GamepadStick {
		int thumb;
		explicit GamepadStick(int thumbCode) : thumb(thumbCode) {}
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

		// Creates new controller with next free index
		void AddController();
		// Creates new controller
		void AddController(int controllerIndex);
		// Adds existing controller
		void AddController(std::unique_ptr<Controller> controller);

		Controller* GetController(int ControllerIndex) const;

		void BindCommand(GamepadStick thumb, std::unique_ptr<Command> command, int controllerIndex = 0) const
		{
			if (Controller* controller = GetController(controllerIndex)) {
				controller->BindThumbCommand(thumb.thumb, std::move(command));
			}
			else {
				std::cerr << "No valid controller index!\n";
				assert(GetController(controllerIndex));
			}
		}

		void BindCommand(GamepadButton button, std::unique_ptr<Command> command, InputActionType actionType = InputActionType::OnPressed, int controllerIndex = 0) const
		{
			if (Controller* controller = GetController(controllerIndex)) {
				controller->BindCommand(button, std::move(command), actionType);
			}
			else {
				std::cerr << "no valid controller index!\n";
				assert(GetController(controllerIndex));
			}
		}

		void BindCommand(KeyboardKey key, std::unique_ptr<Command> command, InputActionType actionType = InputActionType::OnPressed) {
			m_KeyboardCommandMap[key.key] = std::make_pair(std::move(command), actionType);
		}

		size_t GetNumberOfControllers() const { return m_Controllers.size(); }

	private:
		// controllers
		std::vector<std::unique_ptr<Controller>> m_Controllers;

		// Keyboard
		std::map<SDL_Keycode, bool> m_CurrentKeyStateMap;
		std::map<SDL_Keycode, bool> m_PreviousKeyStateMap;

		std::map<int, std::pair<std::unique_ptr<Command>, InputActionType>> m_KeyboardCommandMap;
	};
}
