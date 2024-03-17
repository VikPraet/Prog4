#include "InputManager.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>

bool dae::InputManager::ProcessInput()
{
	// Process gamepad commands
	for (const auto& controller : m_Controllers)
	{
		controller->ProcessInput();
	}

	m_PreviousKeyStateMap = m_CurrentKeyStateMap;

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

	// Process Keyboard commands
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

	return true;
}

void dae::InputManager::AddController() {
	int nextIndex = 0;

	for (const auto& controller : m_Controllers) {
		if (controller->GetIndex() == nextIndex) {
			nextIndex++;
		}
		else {
			break;
		}
	}

	// Add a new controller with the next available index
	m_Controllers.push_back(std::make_unique<Controller>(nextIndex));
}

void dae::InputManager::AddController(int controllerIndex)
{
	if (GetController(controllerIndex) != nullptr) {
		// A controller with this index already exists
		std::cerr << "Controller with index " << controllerIndex << " already added\n";
		assert(false);
		return;
	}

	// If no controller with the same index exists, add the new controller
	m_Controllers.push_back(std::make_unique<Controller>(controllerIndex));
}

void dae::InputManager::AddController(std::unique_ptr<Controller> controller)
{
	if (controller) {
		Controller* existingController = GetController(controller->GetIndex());
		if (existingController == nullptr) {
			// No existing controller with this index, safe to add
			m_Controllers.push_back(std::move(controller));
		}
		else {
			std::cerr << "Controller with index " << controller->GetIndex() << " already added\n";
			assert(controller->GetIndex() != controller->GetIndex());
		}
	}
}

Controller* dae::InputManager::GetController(int ControllerIndex) const
{
	for (const auto& controller : m_Controllers) {
		if (controller && controller->GetIndex() == ControllerIndex) {
			return controller.get();
		}
	}
	return nullptr;
}
