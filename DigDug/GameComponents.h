#pragma once
#include "Components.h"
#include "Event.h"

namespace game
{
	class PlayerComponent final : public dae::Component
	{
	public:
		PlayerComponent(dae::GameObject* pOwner, int playerNr, int lives = 3);

		void AddObserver(dae::IObserver<game::EventType>* obs);
		void RemoveObserver(dae::IObserver<game::EventType>* obs);
		int GetLives() const { return m_Lives; }

		void LoseLife()
		{
			--m_Lives;
			m_pPlayerSubject->Notify(EventType::PLAYER_DIED);
			if (m_Lives <= 0)
			{
				m_pPlayerSubject->Notify(EventType::GAME_OVER);
			}
		}
	private:
		const int m_PlayerNr;
		int m_Lives;

		std::unique_ptr<dae::Subject<game::EventType>> m_pPlayerSubject{};
	};
}