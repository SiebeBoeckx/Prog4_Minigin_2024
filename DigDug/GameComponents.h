#pragma once
#include "Components.h"
#include "Event.h"

namespace game
{
	class PlayerComponent final : public dae::Component
	{
	public:
		PlayerComponent(dae::GameObject* pOwner, int playerNr, int lives = 3);

		virtual void Update(float) override;

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
		std::vector<dae::ColliderComponent*> m_pColliders{};
	};

	class MoveableComponent final : public dae::Component
	{
	public:
		MoveableComponent(dae::GameObject* pOwner);

		virtual void Update(float dt) override;
		void SetTargetDir(glm::vec2 dir) { m_TargetDir = dir; };
		const glm::vec2 GetPrevDir() const { return m_PrevDir; };

		float m_MoveSpeed{10.f};
	private:
		dae::GameObject* m_pOwner{ nullptr }; //Store again here instead of in Comp so i can access non-const functions
		glm::vec2 m_PrevDir{0.f, 0.f};
		glm::vec2 m_TargetDir{0.f, 0.f}; 
		std::vector<dae::ColliderComponent*> m_pColliders{};
		dae::ColliderComponent* m_pOwnerCollider{};

		void Move(float dt);
	};
}