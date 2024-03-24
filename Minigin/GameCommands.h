#pragma once
#include "Command.h"
#include "Components.h"

class MoveCommand : public dae::Command
{
public:
	MoveCommand(dae::GameObject* pOwner, glm::vec2 dir, float moveSpeed = 10.f) : Command(), m_Dir{ dir }, m_pOwner{ pOwner }, m_MoveSpeed{ moveSpeed } {};
	virtual void Execute(float deltaT) override;
private:
	dae::GameObject* m_pOwner{};
	glm::vec2 m_Dir{};
	const float m_MoveSpeed{ 10.f };
};