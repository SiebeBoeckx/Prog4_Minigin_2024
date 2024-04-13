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

		struct KeyboardAction
		{
			KeyState state{};
			std::unique_ptr<Command> command{};

			SDL_Scancode key{};
		};

		struct ControllerAction
		{
			KeyState state{};
			std::unique_ptr<Command> command{};
			int controllerIdx{};

			XBox360Controller::Button controllerButton{};
		};
		virtual ~InputManager() = default;
		bool ProcessInput(float deltaT);
		void UpdateControllers();
		bool UpdateMouse();
		void ExecuteActions(float deltaT);

		int AddController();

		bool IsPressed(XBox360Controller::Button button, int controllerIdx) const;
		bool IsDownThisFrame(XBox360Controller::Button button, int controllerIdx) const;
		bool IsUpThisFrame(XBox360Controller::Button button, int controllerIdx) const;
		glm::ivec2 GetMousePos()const { return m_MousePos; };
		bool IsMousePressed() const { return m_isMousePressed; };

		//void AddDualCommand(XBox360Controller::Button button, SDL_Scancode keyboardButton, std::unique_ptr<Command> command, int playerIdx, KeyState state = KeyState::Down);
		void AddKeyboardCommand(SDL_Scancode keyboardButton, std::unique_ptr<Command> command, KeyState state = KeyState::Down);
		void AddControllerCommand(XBox360Controller::Button button, std::unique_ptr<Command> command, int controllerIdx, KeyState state = KeyState::Down);

		void RemoveKeyboardCommand(std::unique_ptr<KeyboardAction> pCommand);
		void RemoveControllerCommand(std::unique_ptr<ControllerAction> pCommand);

		XBox360Controller& GetController(int idx);

	private:
		std::vector<std::unique_ptr<XBox360Controller>> m_pControllers{};
		std::vector<std::unique_ptr<KeyboardAction>> m_pKeyCommands{};
		std::vector<std::unique_ptr<ControllerAction>> m_pControllerCommands{};

		glm::ivec2 m_MousePos{};
		bool m_isMousePressed{ false };

		Uint8 m_PreviousKeyboardState[SDL_NUM_SCANCODES]{ };
	};

}
