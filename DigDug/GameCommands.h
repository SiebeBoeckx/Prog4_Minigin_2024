#pragma once
#include "Command.h"
#include "GameComponents.h"
#include "GameSystems.h"
namespace game
{
	class MoveCommand : public dae::Command
	{
	public:
		MoveCommand(dae::GameObject* pOwner, glm::vec2 dir, float moveSpeed = 10.f);
		virtual void Execute(float dt) override;
	private:
		dae::GameObject* m_pOwner{};
		game::MoveableComponent* m_pOwnerMoveableComp{ nullptr };
		const glm::vec2 m_Dir{};
		const float m_MoveSpeed{ 10.f };
	};

	class PickupItem_Command_P1 : public dae::Command
	{
	public:
		PickupItem_Command_P1(ScoreSystem* scoreSystem) : Command(), m_pScoreSystem{ scoreSystem } {};
		virtual void Execute(float) override;
	private:
		ScoreSystem* m_pScoreSystem{};
	};

	class PickupItem_Command_P2 : public dae::Command
	{
	public:
		PickupItem_Command_P2(ScoreSystem* scoreSystem) : Command(), m_pScoreSystem{ scoreSystem } {};
		virtual void Execute(float) override;
	private:
		ScoreSystem* m_pScoreSystem{};
	};

	class LoseLifeCommand : public dae::Command
	{
	public:
		LoseLifeCommand(dae::GameObject* pOwner) : Command(), m_pOwner{ pOwner } {};
		virtual void Execute(float) override;
	private:
		dae::GameObject* m_pOwner{};
	};
}