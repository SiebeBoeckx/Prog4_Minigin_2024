#include "Enemies.h"
#include "GameObject.h"

// Define the static members
game::SearchingState game::PookaState::m_Searching(nullptr);
game::GhostState game::PookaState::m_Ghost(nullptr);        

game::PookaState::PookaState(PookaComponent* pPooka)
	:m_pPooka{ pPooka }
{
	m_Searching.m_pPooka = pPooka;
	m_Ghost.m_pPooka = pPooka;
}

game::SearchingState::SearchingState(PookaComponent* pPooka)
	:PookaState(pPooka)
{
}

void game::SearchingState::Update(float dt)
{
	m_pPooka->GetOwner()->Translate(m_PrevDir * m_MoveSpeed * dt);
}

void game::SearchingState::OnEnter()
{
}

game::GhostState::GhostState(PookaComponent* pPooka)
	:PookaState(pPooka)
{
}

void game::GhostState::Update(float)
{
}

void game::GhostState::OnEnter()
{
}

game::PookaComponent::PookaComponent(dae::GameObject* pOwner, float size)
	: Component(pOwner)
	, m_Size{ size }
	, m_pState{ new SearchingState(this) }
{
	m_pCollider = &pOwner->AddComponent<dae::ColliderComponent>("ENEMY");
	m_pCollider->SetDimensions(size, size);
}