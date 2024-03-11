#include "InputManager.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>

bool dae::InputManager::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	for (const auto& [buttonCode, commandPair] : m_CommandMap) {
		const bool isPressed = m_CurrentState.Gamepad.wButtons & buttonCode;
		const bool wasPressed = m_PreviousState.Gamepad.wButtons & buttonCode;

		switch (const InputActionType actionType = commandPair.second) {
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
