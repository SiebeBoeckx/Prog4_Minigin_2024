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
		void SetStartPos(glm::vec2 pos) { m_StartPos = pos; };

		void LoseLife();	
	private:
		const int m_PlayerNr;
		int m_Lives;

		std::unique_ptr<dae::Subject<game::EventType>> m_pPlayerSubject{};
		std::vector<dae::ColliderComponent*> m_pColliders{};
		glm::vec2 m_StartPos{};
	};

	class MoveableComponent final : public dae::Component
	{
	public:
		MoveableComponent(dae::GameObject* pOwner);

		virtual void Update(float) override;
		void SetTargetDir(glm::vec2 dir) { m_TargetDir = dir; };
		const glm::vec2 GetPrevDir() const { return m_PrevDir; };
		void Move(float dt);

		float m_MoveSpeed{10.f};
	private:
		dae::GameObject* m_pOwner{ nullptr }; //Store again here instead of in Comp so i can access non-const functions
		glm::vec2 m_PrevDir{0.f, 0.f};
		glm::vec2 m_TargetDir{0.f, 0.f}; 
		std::vector<dae::ColliderComponent*> m_pColliders{};
		dae::ColliderComponent* m_pOwnerCollider{};
		glm::vec2 m_PrevPos{ 0.f, 0.f };
	};

	class PumpComponent final : public dae::Component
	{
	public:
		PumpComponent(dae::GameObject* pOwner);

		virtual void Update(float dt) override;
		void Fire(glm::vec2 direction);
		void Hold();

		void SetRange(float range) { m_Range = range; };
		bool GetIsActive() const { return m_IsActive != m_IsHooked; }; //XOR
	private:
		dae::GameObject* m_pOwner{ nullptr }; //Store again here instead of in Comp so i can access non-const functions
		std::vector<dae::ColliderComponent*> m_pColliders{};
		dae::ColliderComponent* m_pOwnerCollider{};
		dae::TextureComponent* m_pOwnerTexture{};
		const float m_MoveSpeed{ 70.f };
		float m_Range{0.f};
		glm::vec2 m_Dir{};
		float m_CurrentOffset{ 0.f };
		bool m_IsActive{ false };

		bool m_IsHooked{ false };
		float m_TimeHooked{ 0.f };
		const float m_MaxTimeHooked{ 0.5f };
		dae::GameObject* m_pHookedEnemy{ nullptr };

		void ResetHook();
	};
}