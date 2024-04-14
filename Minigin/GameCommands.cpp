#include "GameCommands.h"
#include "GameObject.h"
#include "Displays.h"
#include "CollisionManager.h"


using namespace dae;

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

	const glm::vec3 curPos = collider->GetPosition();
	const glm::vec3 newPos = curPos + glm::vec3{ m_Dir * m_MoveSpeed * deltaT, 0 };
	collider->SetPosition(newPos.x, newPos.y);

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
	m_pOwner->GetComponent<PlayerComponent>()->LoseLife();
}
