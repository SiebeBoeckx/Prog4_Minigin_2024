#include "GameCommands.h"
#include "GameObject.h"
#include "Displays.h"


using namespace dae;

void MoveCommand::Execute(float deltaT)
{
	if (!m_pOwner)
	{
		return;
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
