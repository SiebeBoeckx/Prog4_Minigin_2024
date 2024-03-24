#pragma once
#include "Displays.h"
#include "GameObject.h"

#pragma region LivesDisplay

LivesDisplayComponent::LivesDisplayComponent(dae::GameObject* pOwner, dae::PlayerComponent* player)
	:Component(pOwner)
	, m_pTextComponent{ GetOwner()->GetComponent<dae::TextComponent>() }
	, m_Text{ "Lives: " }
	, m_pPlayer(player)
{
}

//LivesDisplayComponent::~LivesDisplayComponent()
//{
//	m_pPlayer->RemoveObserver(this);
//}

void LivesDisplayComponent::HandleEvent(EventType event)
{
	switch (event)
	{
	case EventType::START_GAME:
		m_Text = "LIVES: " + std::to_string(m_pPlayer->GetLives());
		m_pTextComponent->SetText(m_Text);
		break;
	case EventType::PLAYER_DIED:
		m_Text = "LIVES: " + std::to_string(m_pPlayer->GetLives());
		m_pTextComponent->SetText(m_Text);
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region ScoreDisplay

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* pOwner, dae::PlayerComponent* player)
	:Component(pOwner)
	, pTextComponent{ GetOwner()->GetComponent<dae::TextComponent>() }
	, m_Text{ "SCORE: " }
	, m_pPlayer(player)
{
}

//ScoreDisplayComponent::~ScoreDisplayComponent()
//{
//	if (m_pPlayer)
//	{
//		m_pPlayer->RemoveObserver(this);
//	}
//}

void ScoreDisplayComponent::HandleEvent(EventType event)
{
	switch (event)
	{
	case EventType::ADD_POINTS:
		m_Score += 10;
		m_Text = "SCORE: " + std::to_string(m_Score);
		pTextComponent->SetText(m_Text);
		break;
	case EventType::START_GAME:
		m_Text = "SCORE: " + std::to_string(m_Score);
		pTextComponent->SetText(m_Text);
	default:
		break;
	}
}
#pragma endregion