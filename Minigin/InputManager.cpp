#include "InputManager.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>

bool dae::InputManager::ProcessInput()
{
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
