#pragma once
#include <memory>
#include "Singleton.h"
#include "Subject.h"
namespace dae
{
	class ScoreSystem : public Singleton<ScoreSystem>, public IObserver
	{
	public:
		void HandleEvent(EventType event) override;
		const int GetScore_P1() { return m_PointsPlayer1; }
		const int GetScore_P2() { return m_PointsPlayer2; }

		void AddObserver_P1(IObserver* obs);
		void RemoveObserver_P1(IObserver* obs);
		void AddObserver_P2(IObserver* obs);
		void RemoveObserver_P2(IObserver* obs);

	private:
		friend class Singleton<ScoreSystem>;
		ScoreSystem()
		{
			m_pScoreSubject1 = std::make_unique<Subject>();
			m_pScoreSubject2 = std::make_unique<Subject>();
		}

		int m_PointsPlayer1{0};
		int m_PointsPlayer2{0};
		std::unique_ptr<Subject> m_pScoreSubject1{};
		std::unique_ptr<Subject> m_pScoreSubject2{};
	};

}