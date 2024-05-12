#include "GameCommands.h"
#include "GameObject.h"
#include "Displays.h"
#include "CollisionManager.h"
#include <iostream>


using namespace dae;

namespace game
{
	void MoveCommand::Execute(float deltaT)
	{
		if (!m_pOwner)
		{
			return;
		}
		ColliderComponent* collider = m_pOwner->GetComponent<ColliderComponent>();
		if (!collider)
		{
			return;
		}
		const auto& collisions = CollisionManager::GetInstance().GetColliders();

		const glm::vec2 curPos = collider->GetLocalPosition();
		const glm::vec2 newPos = curPos + glm::vec2{ m_Dir * m_MoveSpeed * deltaT };
		collider->SetPosition(newPos.x, newPos.y);
		collider->Update(0.f);

		for (auto collision : collisions)
		{
			//skipping same collision
			if (collision == collider)
			{
				continue;
			}

			if (collider->IsColliding(collision))
			{
				//std::cout << "Colliding\n";
				// if is colliding with something after an update, don't update movement!
				collider->SetPosition(curPos.x, curPos.y);
				return;
			}
		}
		//std::cout << curPos.x << ", " << curPos.y << '\n';
		//std::cout << m_pOwner->GetGlobalTransform()->GetPosition().x << ", " << m_pOwner->GetGlobalTransform()->GetPosition().y << ", " << m_pOwner->GetGlobalTransform()->GetPosition().z << '\n';
		collider->SetPosition(curPos.x, curPos.y);
		m_pOwner->Translate(m_Dir * m_MoveSpeed * deltaT);
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
	}
}