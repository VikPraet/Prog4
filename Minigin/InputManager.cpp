#include "InputManager.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>

bool dae::InputManager::ProcessInput()
{
	CopyMemory(&m_PreviousGamepadState, &m_CurrentGamepadState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentGamepadState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentGamepadState);

	// Process gamepad Commands
	for (const auto& [buttonCode, commandPair] : m_GamepadCommandMap) {
		const bool isPressed = m_CurrentGamepadState.Gamepad.wButtons & buttonCode;
		const bool wasPressed = m_PreviousGamepadState.Gamepad.wButtons & buttonCode;

		switch (commandPair.second) {
		case InputActionType::OnRelease:
			if (!isPressed && wasPressed) {
				commandPair.first->Execute();
			}
			break;

		case InputActionType::OnPressed:
			if (isPressed && !wasPressed) {
				commandPair.first->Execute();
			}
			break;

		case InputActionType::Continuous:
			if (isPressed) {
				commandPair.first->Execute();
			}
			break;
		}
	}


	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		// process ImGui input
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type) {
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:
			m_CurrentKeyStateMap[e.key.keysym.sym] = true;
			break;

		case SDL_KEYUP:
			m_CurrentKeyStateMap[e.key.keysym.sym] = false;
			break;
		}
	}

	// Process Keyboard Commands
	for (const auto& [key, commandPair] : m_KeyboardCommandMap) {
		const bool isPressed = m_CurrentKeyStateMap[key];
		const bool wasPressed = m_PreviousKeyStateMap.contains(key) ? m_PreviousKeyStateMap[key] : false;

		switch (commandPair.second) {
		case InputActionType::OnRelease:
			if (!isPressed && wasPressed) {
				commandPair.first->Execute();
			}
			break;

		case InputActionType::OnPressed:
			if (isPressed && !wasPressed) {
				commandPair.first->Execute();
			}
			break;

		case InputActionType::Continuous:
			if (isPressed) {
				commandPair.first->Execute();
			}
			break;
		}
	}

	m_PreviousKeyStateMap = m_CurrentKeyStateMap;

	return true;
}
