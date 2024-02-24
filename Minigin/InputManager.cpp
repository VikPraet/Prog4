#include <SDL.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
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
		// etc...
	}

	return true;
}
