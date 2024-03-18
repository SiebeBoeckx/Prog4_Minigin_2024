#pragma once
#ifndef COMMAND_H
#define COMMAND_H

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void Execute() = 0;
	};

	class EditorCommand : public Command
	{
	public:
		virtual void Undo() = 0;
	};
}

#endif // !COMMAND