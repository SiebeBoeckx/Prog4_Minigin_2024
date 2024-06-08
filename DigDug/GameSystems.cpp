#include "GameSystems.h"
#include "ServiceLocator.h"
#include "Scene.h"

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

		case EventType::KILL_POOKA_1_P1:
			m_PointsPlayer1 += 200;
			m_pScoreSubject1->Notify(ADD_POINTS);
			break;
		case EventType::KILL_POOKA_2_P1:
			m_PointsPlayer1 += 300;
			m_pScoreSubject1->Notify(ADD_POINTS);
			break;
		case EventType::KILL_POOKA_3_P1:
			m_PointsPlayer1 += 400;
			m_pScoreSubject1->Notify(ADD_POINTS);
			break;
		case EventType::KILL_POOKA_4_P1:
			m_PointsPlayer1 += 500;
			m_pScoreSubject1->Notify(ADD_POINTS);
			break;

		case EventType::KILL_POOKA_1_P2:
			m_PointsPlayer2 += 200;
			m_pScoreSubject2->Notify(ADD_POINTS);
			break;
		case EventType::KILL_POOKA_2_P2:
			m_PointsPlayer2 += 300;
			m_pScoreSubject2->Notify(ADD_POINTS);
			break;
		case EventType::KILL_POOKA_3_P2:
			m_PointsPlayer2 += 400;
			m_pScoreSubject2->Notify(ADD_POINTS);
			break;
		case EventType::KILL_POOKA_4_P2:
			m_PointsPlayer2 += 500;
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


	void SceneSystem::HandleEvent(game::EventType event)
	{
		switch (event)
		{
		case EventType::GAME_WIN:
			m_pScene->TogglePauseScene(true);
			break;
		case EventType::GAME_OVER:
			m_pScene->TogglePauseScene(true);
			break;
		default:
			break;
		}
	}
}