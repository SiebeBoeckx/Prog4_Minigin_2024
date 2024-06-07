#include "GameSystems.h"
#include "ServiceLocator.h"

namespace game
{
	void ScoreSystem::HandleEvent(EventType event)
	{
		switch (event)
		{
		case EventType::PICKUP_ITEM_P1:
			m_PointsPlayer1 += 10;
			m_pScoreSubject1->Notify(ADD_POINTS);
			dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Resources/Sounds/Pickup.mp3", 10, 0);
			break;
		case EventType::PICKUP_ITEM_P2:
			m_PointsPlayer2 += 10;
			m_pScoreSubject2->Notify(ADD_POINTS);
			dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/Resources/Sounds/Pickup.mp3", 10, 0);
			break;

		case EventType::DIG_P1:
			m_PointsPlayer1 += 5;
			m_pScoreSubject1->Notify(ADD_POINTS);
			break;
		case EventType::DIG_P2:
			m_PointsPlayer2 += 5;
			m_pScoreSubject2->Notify(ADD_POINTS);
			break;
		default:
			break;
		}
	}

	void ScoreSystem::AddObserver_P1(IObserver* obs)
	{
		m_pScoreSubject1->AddObserver(obs);
	}

	void ScoreSystem::RemoveObserver_P1(IObserver* obs)
	{
		m_pScoreSubject1->RemoveObserver(obs);
	}

	void ScoreSystem::AddObserver_P2(IObserver* obs)
	{
		m_pScoreSubject2->AddObserver(obs);
	}

	void ScoreSystem::RemoveObserver_P2(IObserver* obs)
	{
		m_pScoreSubject2->RemoveObserver(obs);
	}
}