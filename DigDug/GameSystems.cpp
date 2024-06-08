#include "GameSystems.h"
#include "ServiceLocator.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <string>

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
			GetInstance().SetHighScore(ScoreSystem::GetInstance().GetScore_P1());
			GetInstance().SetHighScore(ScoreSystem::GetInstance().GetScore_P2());
			break;
		case EventType::GAME_OVER:
			m_pScene->TogglePauseScene(true);
			GetInstance().SetHighScore(ScoreSystem::GetInstance().GetScore_P1());
			GetInstance().SetHighScore(ScoreSystem::GetInstance().GetScore_P2());
			break;
		default:
			break;
		}
	}
	void SceneSystem::SetHighScore(int highScore)
	{
		m_HighScores.push_back(highScore);
		std::sort(m_HighScores.begin(), m_HighScores.end(), std::greater<int>());
		if (m_HighScores.size() > 10)
		{
			m_HighScores.resize(10); // Keep only the top 10 scores
		}
		WriteToHighScore();
	}
	void SceneSystem::WriteToHighScore()
	{
		std::ofstream highScoreFile("../Data/highscores.txt");
		if (highScoreFile.is_open())
		{
			for (const int score : m_HighScores)
			{
				highScoreFile << score << std::endl;
			}
			highScoreFile.close();
		}
		else
		{
			std::cerr << "Error: Unable to open highscores.txt file for writing.\n";
		}
	}
	void SceneSystem::ReadHighScore()
	{
		std::ifstream highScoreFile("../Data/highscores.txt");
		if (highScoreFile.is_open())
		{
			std::string line;
			while (std::getline(highScoreFile, line))
			{				
				int score = std::stoi(line);
				m_HighScores.push_back(score);				
			}
			highScoreFile.close();
			std::sort(m_HighScores.begin(), m_HighScores.end(), std::greater<int>());
			if (m_HighScores.size() > 10)
			{
				m_HighScores.resize(10); // Keep only the top 10 scores
			}
		}
		else
		{
			std::cerr << "highscores.txt file not found. Creating a new file.\n";
			std::ofstream newHighScoreFile("../Data/highscores.txt");
			if (newHighScoreFile.is_open())
			{
				newHighScoreFile.close();
			}
			else
			{
				std::cerr << "Error: Unable to create highscores.txt file.\n";
			}
		}
	}
}