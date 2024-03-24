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

class AddPointsCommand : public dae::Command
{
public:
	AddPointsCommand(dae::GameObject* pOwner, int points = 10) : Command(), m_pOwner{ pOwner }, m_Points{ points } {};
	virtual void Execute(float) override;
private:
	dae::GameObject* m_pOwner{};
	int m_Points{};
};

class LoseLifeCommand : public dae::Command
{
public:
	LoseLifeCommand(dae::GameObject* pOwner) : Command(), m_pOwner{ pOwner } {};
	virtual void Execute(float) override;
private:
	dae::GameObject* m_pOwner{};
};