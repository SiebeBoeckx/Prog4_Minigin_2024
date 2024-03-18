#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
//#include <string>

namespace dae
{
	bool InputManager::ProcessInput()
	{
		const bool returnBool = UpdateMouse(); //Returns false to quit
		UpdateControllers();
		ExecuteActions();
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

	void InputManager::ExecuteActions()
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

		for (const auto& keyAction : m_pKeyCommands)
		{
			bool keyPressed = keyboardState[keyAction->key];
			bool keyPreviouslyPressed = m_PreviousKeyboardState[keyAction->key];

			switch (keyAction->state)
			{
			case KeyState::Down:
				if (IsDownThisFrame(keyAction->controllerButton, keyAction->playerIdx))
				{
					keyAction->command->Execute();
				}
				else if (keyPressed && !keyPreviouslyPressed)
				{
					keyAction->command.get()->Execute();
				}
				break;
			case KeyState::Pressed:
				if (IsPressed(keyAction->controllerButton, keyAction->playerIdx))
				{
					keyAction->command->Execute();
				}
				else if (keyPressed)
				{
					keyAction->command.get()->Execute();
				}
				break;
			case KeyState::Up:
				if (IsUpThisFrame(keyAction->controllerButton, keyAction->playerIdx))
				{
					keyAction->command->Execute();
				}
				else if (!keyPressed && keyPreviouslyPressed)
				{
					keyAction->command.get()->Execute();
				}
				break;
			default:
				break;
			}
		}

		// Update previous keyboard states
		std::memcpy(m_PreviousKeyboardState, keyboardState, SDL_NUM_SCANCODES);
	}

	//returns size of m_pControllers aka the last added player what their number is.
	//size before push_back is same as index after.
	//ex, 2 players in vector, size = 2 while the last index is 1. After push back, the new controller would have index 2.
	int InputManager::AddPlayer()
	{
		int idx = static_cast<int>(m_pControllers.size());
		m_pControllers.push_back(std::make_unique<XBox360Controller>(idx));
		return idx;
	}

	// returns a player, if the idx is out-of-range it takes the last added player
	XBox360Controller& InputManager::GetPlayer(int idx)
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

	bool InputManager::IsPressed(XBox360Controller::Button button, int  playerIdx) const
	{
		//if (button == XBox360Controller::Button::DPadDown)
		//{
		//	std::cout << "Hello" << '\n';
		//}
		if (button != XBox360Controller::Button::Default)
		{
			return m_pControllers[playerIdx]->IsBeingPressed(button);
		}
		return false;
	}

	bool InputManager::IsDownThisFrame(XBox360Controller::Button button, int playerIdx) const
	{
		if (button != XBox360Controller::Button::Default)
		{
			return m_pControllers[playerIdx]->IsDown(button);
		}
		return false;
	}

	bool InputManager::IsUpThisFrame(XBox360Controller::Button button, int playerIdx) const
	{
		if (button != XBox360Controller::Button::Default)
		{
			return m_pControllers[playerIdx]->IsUp(button);
		}
		return false;
	}

	void InputManager::AddCommand(XBox360Controller::Button controllerButton, SDL_Scancode keyboardButton, std::unique_ptr<Command> command, int playerIdx, KeyState state)
	{
		if (playerIdx >= static_cast<int>(m_pControllers.size()))
		{
			std::cout << "Player not found, cannot add command\n\n";
			return;
		}
		std::unique_ptr<KeyAction> action = std::make_unique<KeyAction>();
		action->command = std::move(command);
		action->controllerButton = controllerButton;
		action->state = state;
		action->playerIdx = playerIdx;
		action->key = keyboardButton;
		m_pKeyCommands.emplace_back(std::move(action));
	}

	void InputManager::AddCommand(SDL_Scancode keyboardButton, std::unique_ptr<Command> command, int playerIdx, KeyState state)
	{
		AddCommand(XBox360Controller::Button::Default, keyboardButton, std::move(command), playerIdx, state);
	}

	void InputManager::AddCommand(XBox360Controller::Button button, std::unique_ptr<Command> command, int playerIdx, KeyState state)
	{
		AddCommand(button, SDL_SCANCODE_UNKNOWN, std::move(command), playerIdx, state);
	}

	void InputManager::RemoveCommand(std::unique_ptr<KeyAction> pCommand)
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
}
