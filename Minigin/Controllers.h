#pragma once
#include "Singleton.h"

namespace dae
{
	class XBox360Controller final : public Singleton<XBox360Controller>
	{
		class XBox360ControllerImpl;
		XBox360ControllerImpl* pImpl;
	public:
		enum class Button
		{
			Default = 0x0000,
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000
		};

		void Update();

		bool IsDown(Button button) const;
		bool IsUp(Button button) const;
		bool IsBeingPressed(Button button) const;

		explicit XBox360Controller(unsigned int controllerIdx);
		~XBox360Controller();

		XBox360Controller(const XBox360Controller& other) = delete;
		XBox360Controller(XBox360Controller&& other) = delete;
		XBox360Controller& operator=(const XBox360Controller& other) = delete;
		XBox360Controller& operator=(XBox360Controller&& other) = delete;

		unsigned int idx{};
	};
}
