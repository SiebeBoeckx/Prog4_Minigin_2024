#include "GameCommands.h"
#include "GameObject.h"
#include "Displays.h"
#include "CollisionManager.h"
#include "Wall.h"
#include <iostream>
#include "Enemies.h"
#include "GameSystems.h"
#include "Scene.h"

using namespace dae;

namespace game
{
	MoveCommand::MoveCommand(dae::GameObject* pOwner, glm::vec2 dir, float moveSpeed)
		: Command()
		, m_Dir{ dir }
		, m_pOwner{ pOwner }
		, m_MoveSpeed{ moveSpeed }
	{
		m_pOwnerMoveableComp = pOwner->GetComponent<game::MoveableComponent>();
	};
	void MoveCommand::Execute(float dt)
	{
		if(game::SceneSystem::GetInstance().GetScene()->GetPaused())
		{
			return;
		}
		if (!m_pOwner)
		{
			return;
		}
		if (!m_pOwnerMoveableComp)
		{
			return;
		}
		m_pOwnerMoveableComp->m_MoveSpeed = m_MoveSpeed;
		m_pOwnerMoveableComp->SetTargetDir(m_Dir);
		m_pOwnerMoveableComp->Move(dt);
	}
	
	void PickupItem_Command_P1::Execute(float)
	{
		m_pScoreSystem->HandleEvent(PICKUP_ITEM_P1);

	}

	void PickupItem_Command_P2::Execute(float)
	{
		m_pScoreSystem->HandleEvent(PICKUP_ITEM_P2);

	}

	void LoseLifeCommand::Execute(float)
	{
		m_pOwner->GetComponent<game::PlayerComponent>()->LoseLife();

		for (auto& collision : CollisionManager::GetInstance().GetColliders())
		{
			if (collision->GetInValid())
			{
				continue;
			}
			if (collision->GetTag() == "ENEMY") //Reset enemies
			{
				collision->GetOwner()->GetComponent<game::EnemyComponent>()->Reset();
				continue;
			}
		}
	}
	FireCommand::FireCommand(dae::GameObject* pOwner, game::MoveableComponent* pPlayerMover)
		:Command()
		,m_pOwner(pOwner)
		,m_pPlayerMoveableComp(pPlayerMover)
	{
		m_pPumpComponent = pOwner->GetComponent<game::PumpComponent>();
	}
	void FireCommand::Execute(float)
	{
		if (game::SceneSystem::GetInstance().GetScene()->GetPaused())
		{
			return;
		}
		m_pPumpComponent->Fire(m_pPlayerMoveableComp->GetPrevDir());
	}

	HoldCommand::HoldCommand(dae::GameObject* pOwner, game::MoveableComponent* pPlayerMover)
		:Command()
		, m_pOwner(pOwner)
		, m_pPlayerMoveableComp(pPlayerMover)
	{
		m_pPumpComponent = pOwner->GetComponent<game::PumpComponent>();
	}

	void HoldCommand::Execute(float)
	{
		if (game::SceneSystem::GetInstance().GetScene()->GetPaused())
		{
			return;
		}
		m_pPumpComponent->Hold();
	}
}