#include "GameCommands.h"
#include "GameObject.h"

using namespace dae;

void MoveCommand::Execute(float deltaT)
{
	if (!m_pOwner)
	{
		return;
	}

	m_pOwner->Translate(m_Dir * m_MoveSpeed * deltaT);
}
