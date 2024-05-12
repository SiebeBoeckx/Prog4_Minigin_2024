#include "Boulder.h"
#include "GameObject.h"

IdleState BoulderState::m_Idle(nullptr);
FallingState BoulderState::m_Falling(nullptr);
BreakState BoulderState::m_Breaking(nullptr);

BoulderState::BoulderState(BoulderComponent* pBoulder)
	:m_pBoulder{ pBoulder }
{
	m_Idle.m_pBoulder = pBoulder;
	m_Falling.m_pBoulder = pBoulder;
	m_Breaking.m_pBoulder = pBoulder;
}

IdleState::IdleState(BoulderComponent* pBoulder)
	:BoulderState(pBoulder)
{
}

void IdleState::Update(float)
{
}

FallingState::FallingState(BoulderComponent* pBoulder)
	:BoulderState(pBoulder)
{
}

void FallingState::Update(float)
{
}

void FallingState::OnEnter()
{
}

BreakState::BreakState(BoulderComponent* pBoulder)
	:BoulderState(pBoulder)
{
}

void BreakState::OnEnter()
{
}


BoulderComponent::BoulderComponent(dae::GameObject* pOwner, float size)
	: Component(pOwner)
	, m_Size{size}
{
	m_pCollider = &pOwner->AddComponent<dae::ColliderComponent>("BOULDER");
	m_pCollider->SetDimensions(size, size);
}

