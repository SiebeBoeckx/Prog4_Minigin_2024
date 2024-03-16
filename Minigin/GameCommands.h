#pragma once
#include "Command.h"
#include "Components.h"

class MoveCommand : public dae::Command
{
public:
	MoveCommand(dae::GameObject* pOwner, Direction dir) : Command(), m_Dir{ dir }, m_pOwner(pOwner) {};
	virtual void Execute() override;
private:
	Direction m_Dir{};
	dae::GameObject* m_pOwner{};
};