#include "InputManager.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>

bool dae::InputManager::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);
	//auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	//auto buttonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	//auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	for (const auto& [buttonCode, commandPair] : m_CommandMap) 
	{
		const bool isPressed = m_CurrentState.Gamepad.wButtons & buttonCode;
		const bool wasPressed = m_PreviousState.Gamepad.wButtons & buttonCode;
		const bool commandOnRelease = commandPair.second;

		if (commandOnRelease && !isPressed && wasPressed) {
			// Execute command on release
			commandPair.first->Execute();
		}
		else if (!commandOnRelease && isPressed && !wasPressed) {
			// Execute command on press
			commandPair.first->Execute();
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		// process input for IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type)
		{
		case SDL_QUIT:
			return false;
			break;

		case SDL_KEYDOWN:
			//if (e.key.keysym.sym == SDLK_SPACE)
			//{
			//
			//}
			break;

		case SDL_MOUSEBUTTONDOWN:

			break;

		default:
			return true;
		}
	}

	return true;
}
