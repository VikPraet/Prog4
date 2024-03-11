#pragma once
#include <map>
#include <memory>
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

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void BindCommand(int key, std::unique_ptr<Command> command, InputActionType actionType = InputActionType::OnPressed)
		{
			m_CommandMap[key] = std::make_pair(std::move(command), actionType);
		}


	private:
		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};
		int m_ControllerIndex{};

		std::map<int, std::pair<std::unique_ptr<Command>, InputActionType>> m_CommandMap;
	};
}
