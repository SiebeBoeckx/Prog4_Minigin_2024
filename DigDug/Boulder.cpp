#include "Boulder.h"
#include "GameObject.h"

IdleState BoulderState::m_Idle(nullptr);

BoulderState::BoulderState(BoulderComponent* pBoulder)
	:m_pBoulder{ pBoulder }
{
	m_Idle.m_pBoulder = pBoulder;
}



IdleState::IdleState(BoulderComponent* pBoulder)
	:BoulderState(pBoulder)
{
}

void IdleState::Update(float)
{
}

BoulderComponent::BoulderComponent(dae::GameObject* pOwner, float size)
	: Component(pOwner)
	, m_Size{size}
{
	m_pCollider = &pOwner->AddComponent<dae::ColliderComponent>("BOULDER");
	m_pCollider->SetDimensions(size, size);
}
