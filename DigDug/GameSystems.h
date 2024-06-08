#pragma once
#include <memory>
#include "Singleton.h"
#include "Subject.h"
#include "Event.h"

namespace dae
{
	class Scene;
}

namespace game
{
	class ScoreSystem : public dae::Singleton<ScoreSystem>, public dae::IObserver<game::EventType>
	{
	public:
		void HandleEvent(game::EventType event) override;
		const int GetScore_P1() { return m_PointsPlayer1; }
		const int GetScore_P2() { return m_PointsPlayer2; }

		void AddObserver_P1(dae::IObserver<game::EventType>* obs);
		void RemoveObserver_P1(dae::IObserver<game::EventType>* obs);
		void AddObserver_P2(dae::IObserver<game::EventType>* obs);
		void RemoveObserver_P2(dae::IObserver<game::EventType>* obs);

	private:
		friend class Singleton<ScoreSystem>;

		ScoreSystem()
		{
			m_pScoreSubject1 = std::make_unique<dae::Subject<game::EventType>>();
			m_pScoreSubject2 = std::make_unique<dae::Subject<game::EventType>>();
		}

		int m_PointsPlayer1{0};
		int m_PointsPlayer2{0};
		std::unique_ptr<dae::Subject<game::EventType>> m_pScoreSubject1{};
		std::unique_ptr<dae::Subject<game::EventType>> m_pScoreSubject2{};
	};

	class SceneSystem : public dae::Singleton<SceneSystem>, public dae::IObserver<game::EventType>
	{
	public:
		void HandleEvent(game::EventType event) override;
		void SetScene(dae::Scene* pScene) { m_pScene = pScene; };
		const dae::Scene* GetScene() const { return m_pScene; };
	private:
		friend class Singleton<SceneSystem>;
		SceneSystem()
		{
		}

		dae::Scene* m_pScene{ nullptr };
	};

}