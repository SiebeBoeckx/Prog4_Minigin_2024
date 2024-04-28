#pragma once
#include "Displays.h"
#include "GameObject.h"

#pragma region LivesDisplay

LivesDisplayComponent::LivesDisplayComponent(dae::GameObject* pOwner, game::PlayerComponent* player)
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

void LivesDisplayComponent::HandleEvent(game::EventType event)
{
	switch (event)
	{
	case game::EventType::START_GAME:
		m_Text = "LIVES: " + std::to_string(m_pPlayer->GetLives());
		m_pTextComponent->SetText(m_Text);
		break;
	case game::EventType::PLAYER_DIED:
		m_Text = "LIVES: " + std::to_string(m_pPlayer->GetLives());
		m_pTextComponent->SetText(m_Text);
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region ScoreDisplay

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* pOwner, game::ScoreSystem* score, int idx)
	:Component(pOwner)
	, pTextComponent{ GetOwner()->GetComponent<dae::TextComponent>() }
	, m_Text{ "SCORE: " }
	, m_pScore{ score }
	, m_Idx{ idx }
{
}

//ScoreDisplayComponent::~ScoreDisplayComponent()
//{
//	if (m_pPlayer)
//	{
//		m_pPlayer->RemoveObserver(this);
//	}
//}

void ScoreDisplayComponent::HandleEvent(game::EventType event)
{
	switch (event)
	{
	case game::EventType::ADD_POINTS:
		UpdateScoreText();
		pTextComponent->SetText(m_Text);
		break;
	case game::EventType::START_GAME:
		UpdateScoreText();
		pTextComponent->SetText(m_Text);
	default:
		break;
	}
}
void ScoreDisplayComponent::UpdateScoreText()
{
	if (m_Idx == 0)
	{
		m_Text = "SCORE: " + std::to_string(m_pScore->GetScore_P1());
	}
	else if (m_Idx == 1)
	{
		m_Text = "SCORE: " + std::to_string(m_pScore->GetScore_P2());
	}
}
#pragma endregion