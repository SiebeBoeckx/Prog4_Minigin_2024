#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
//#include <string>

namespace dae
{
	bool InputManager::ProcessInput(float deltaT)
	{
		const bool returnBool = UpdateMouse(); //Returns false to quit
		UpdateControllers();
		ExecuteActions(deltaT);
		return returnBool;
	}

	void InputManager::UpdateControllers()
	{
		for (const auto& controller : m_pControllers)
		{
			controller->Update();
		}
	}

	bool InputManager::UpdateMouse()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);

			if (e.type == SDL_QUIT)
			{
				return false;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN && m_isMousePressed == false)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					m_isMousePressed = true;
				}
			}
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					m_isMousePressed = false;
				}
			}

			ImGui_ImplSDL2_ProcessEvent(&e);
			//if (e.type == SDL_KEYDOWN)
			//{
			//	for (const auto& keyAction : m_KeyCommands)
			//	{
			//		if (keyAction->state == KeyState::Down)
			//		{
			//			if (e.key.keysym.scancode == keyAction->key)
			//			{
			//				keyAction->command.get()->Execute();
			//			}
			//		}
			//	}
			//}
			//if (e.type == SDL_KEYUP)
			//{
			//	for (const auto& keyAction : m_KeyCommands)
			//	{
			//		if (keyAction->state == KeyState::Up)
			//		{
			//			if (e.key.keysym.scancode == keyAction->key)
			//			{
			//				keyAction->command.get()->Execute();
			//			}
			//		}
			//	}
			//}
		}
		return true;
	}

	void InputManager::ExecuteActions(float deltaT)
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		for (const auto& keyAction : m_pKeyCommands)
		{
			bool keyPressed = keyboardState[keyAction->key];
			bool keyPreviouslyPressed = m_PreviousKeyboardState[keyAction->key];

			switch (keyAction->state)
			{
			case KeyState::Down:
				if (keyPressed && !keyPreviouslyPressed)
				{
					keyAction->command.get()->Execute(deltaT);
				}
				break;
			case KeyState::Pressed:
				if (keyPressed)
				{
					keyAction->command.get()->Execute(deltaT);
				}
				break;
			case KeyState::Up:
				if (!keyPressed && keyPreviouslyPressed)
				{
					keyAction->command.get()->Execute(deltaT);
				}
				break;
			default:
				break;
			}
		}

		// Update previous keyboard states
		std::memcpy(m_PreviousKeyboardState, keyboardState, SDL_NUM_SCANCODES);

		for (const auto& controllerAction : m_pControllerCommands)
		{
			switch (controllerAction->state)
			{
			case KeyState::Down:
				if (IsDownThisFrame(controllerAction->controllerButton, controllerAction->controllerIdx))
				{
					controllerAction->command->Execute(deltaT);
				}
				break;
			case KeyState::Pressed:
				if (IsPressed(controllerAction->controllerButton, controllerAction->controllerIdx))
				{
					controllerAction->command->Execute(deltaT);
				}
				break;
			case KeyState::Up:
				if (IsUpThisFrame(controllerAction->controllerButton, controllerAction->controllerIdx))
				{
					controllerAction->command->Execute(deltaT);
				}
				break;
			default:
				break;
			}
		}
	}

	//returns size of m_pControllers aka the last added player what their number is.
	//size before push_back is same as index after.
	//ex, 2 players in vector, size = 2 while the last index is 1. After push back, the new controller would have index 2.
	int InputManager::AddController()
	{
		int idx = static_cast<int>(m_pControllers.size());
		m_pControllers.push_back(std::make_unique<XBox360Controller>(idx));
		return idx;
	}

	// returns a player, if the idx is out-of-range it takes the last added player
	XBox360Controller& InputManager::GetController(int idx)
	{
		if (idx <= static_cast<int>(m_pControllers.size()))
		{
			return *m_pControllers.at(idx);
		}
		else
		{
			return *m_pControllers.back();
		}
	}

	bool InputManager::IsPressed(XBox360Controller::Button button, int controllerIdx) const
	{
		//if (button == XBox360Controller::Button::DPadDown)
		//{
		//	std::cout << "Hello " << playerIdx << '\n';
		//}
		if (button != XBox360Controller::Button::Default)
		{
			//std::cout << "Hello " << playerIdx << '\n';
			//if (playerIdx != 0)
			//{
			//	return m_pControllers[(m_pControllers.size() - 1) - playerIdx]->IsBeingPressed(button);
			//}
			return m_pControllers[controllerIdx]->IsBeingPressed(button);
		}

		return false;
	}

	bool InputManager::IsDownThisFrame(XBox360Controller::Button button, int controllerIdx) const
	{
		if (button != XBox360Controller::Button::Default)
		{
			//if (playerIdx != 0)
			//{
			//	return m_pControllers[(m_pControllers.size() - 1) - playerIdx]->IsDown(button);
			//}
			return m_pControllers[controllerIdx]->IsDown(button);
		}
		return false;
	}

	bool InputManager::IsUpThisFrame(XBox360Controller::Button button, int controllerIdx) const
	{
		if (button != XBox360Controller::Button::Default)
		{
			//if (playerIdx != 0)
			//{
			//	return m_pControllers[(m_pControllers.size() - 1) - playerIdx]->IsUp(button);
			//}
			return m_pControllers[controllerIdx]->IsUp(button);
		}
		return false;
	}

	//void InputManager::AddDualCommand(XBox360Controller::Button controllerButton, SDL_Scancode keyboardButton, std::unique_ptr<Command> command, int playerIdx, KeyState state)
	//{
	//	AddKeyboardCommand(keyboardButton, std::move(command), playerIdx, state);
	//	AddControllerCommand(controllerButton, std::move(command), playerIdx, state);
	//}

	void InputManager::AddKeyboardCommand(SDL_Scancode keyboardButton, std::unique_ptr<Command> command, KeyState state)
	{
		//No longer needed for keyboard
		
		//if (playerIdx >= static_cast<int>(m_pControllers.size()))
		//{
		//	std::cout << "Player not found, cannot add command\n\n";
		//	return;
		//}
		std::unique_ptr<KeyboardAction> action = std::make_unique<KeyboardAction>();
		action->command = std::move(command);
		action->state = state;
		action->key = keyboardButton;
		m_pKeyCommands.emplace_back(std::move(action));
	}

	void InputManager::AddControllerCommand(XBox360Controller::Button button, std::unique_ptr<Command> command, int controllerIdx, KeyState state)
	{
		if (controllerIdx >= static_cast<int>(m_pControllers.size()))
		{
			std::cout << "Player not found, cannot add command\n\n";
			return;
		}
		std::unique_ptr<ControllerAction> action = std::make_unique<ControllerAction>();
		action->command = std::move(command);
		action->state = state;
		action->controllerIdx = controllerIdx;
		action->controllerButton = button;
		m_pControllerCommands.emplace_back(std::move(action));
	}

	void InputManager::RemoveKeyboardCommand(std::unique_ptr<KeyboardAction> pCommand)
	{
		for (auto it{ m_pKeyCommands.begin() }; it != m_pKeyCommands.end(); ++it)
		{
			if (it->get() == pCommand.get())
			{
				m_pKeyCommands.erase(it);
				break;
			}
		}
	}

	void InputManager::RemoveControllerCommand(std::unique_ptr<ControllerAction> pCommand)
	{
		for (auto it{ m_pControllerCommands.begin() }; it != m_pControllerCommands.end(); ++it)
		{
			if (it->get() == pCommand.get())
			{
				m_pControllerCommands.erase(it);
				break;
			}
		}
	}
}
