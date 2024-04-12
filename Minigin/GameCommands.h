#pragma once
#include "Command.h"
#include "Components.h"
#include "GameSystems.h"

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

class PickupItem_Command_P1 : public dae::Command
{
public:
	PickupItem_Command_P1(dae::ScoreSystem* scoreSystem) : Command(), m_pScoreSystem{ scoreSystem } {};
	virtual void Execute(float) override;
private:
	dae::ScoreSystem* m_pScoreSystem{};
};

class PickupItem_Command_P2 : public dae::Command
{
public:
	PickupItem_Command_P2(dae::ScoreSystem* scoreSystem) : Command(), m_pScoreSystem{ scoreSystem } {};
	virtual void Execute(float) override;
private:
	dae::ScoreSystem* m_pScoreSystem{};
};

class LoseLifeCommand : public dae::Command
{
public:
	LoseLifeCommand(dae::GameObject* pOwner) : Command(), m_pOwner{ pOwner } {};
	virtual void Execute(float) override;
private:
	dae::GameObject* m_pOwner{};
};