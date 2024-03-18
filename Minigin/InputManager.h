#pragma once
#include "Singleton.h"
#include "Controllers.h"
#include "Command.h"
#include <memory>
#include <vector>
#include <SDL_scancode.h>
#include <glm/ext/vector_int2.hpp>

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class MouseButton
		{
			Left, Middle, Right
		};
		enum class KeyState
		{
			Down,
			Pressed,
			Up
		};

		struct KeyAction
		{
			KeyState state{};
			std::unique_ptr<Command> command{};
			int playerIdx{};

			XBox360Controller::Button controllerButton{};
			SDL_Scancode key{};
		};
		virtual ~InputManager() = default;
		bool ProcessInput();
		void UpdateControllers();
		bool UpdateMouse();
		void ExecuteActions();

		int AddPlayer();

		bool IsPressed(XBox360Controller::Button button, int playerIdx) const;
		bool IsDownThisFrame(XBox360Controller::Button button, int playeIdx) const;
		bool IsUpThisFrame(XBox360Controller::Button button, int playerIdx) const;
		glm::ivec2 GetMousePos()const { return m_MousePos; };
		bool IsMousePressed() const { return m_isMousePressed; };

		void AddCommand(XBox360Controller::Button button, SDL_Scancode keyboardButton, std::unique_ptr<Command> command, int playerIdx, KeyState state = KeyState::Down);
		void AddCommand(SDL_Scancode keyboardButton, std::unique_ptr<Command> command, int playerIdx, KeyState state = KeyState::Down);
		void AddCommand(XBox360Controller::Button button, std::unique_ptr<Command> command, int playerIdx, KeyState state = KeyState::Down);

		XBox360Controller& GetPlayer(int idx);

	private:
		std::vector<std::unique_ptr<XBox360Controller>> m_pControllers{};
		std::vector<std::unique_ptr<KeyAction>> m_pKeyCommands{};

		glm::ivec2 m_MousePos{};
		bool m_isMousePressed{ false };

		Uint8 m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ };
	};

}
