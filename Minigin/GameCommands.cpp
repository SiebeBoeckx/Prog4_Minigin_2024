#include "GameCommands.h"
#include "GameObject.h"

using namespace dae;

void MoveCommand::Execute()
{
	PlayerComponent* myPlayer = m_pOwner->GetComponent<PlayerComponent>();

	if (!myPlayer)
	{
		return;
	}

	myPlayer->Move(m_Dir);
}
