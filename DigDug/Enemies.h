#pragma once
#include "Components.h"

namespace dae
{
	class Scene;
}

namespace game
{
	class EnemyComponent : public dae::Component
	{
	public:
		EnemyComponent(dae::GameObject* pOwner);
		virtual ~EnemyComponent() = default;

		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;

		virtual void Update(float dt) = 0;

		virtual void SetStartPos(glm::vec2 startPos) { m_StartPos = startPos; };
		virtual void Reset();
		dae::ColliderComponent* GetCollider() const { return m_pCollider; };
		virtual void AddAir(float amount) = 0;
	protected:
		dae::ColliderComponent* m_pCollider{ nullptr };
		glm::vec2 m_StartPos{};
	};

	class PookaComponent; // Forward declaration

	class PookaState
	{
	protected:
		PookaComponent* m_pPooka{ nullptr };
	public:
		PookaState(PookaComponent* pPooka);
		virtual ~PookaState() = default;

		PookaState(const PookaState& other) = delete;
		PookaState(PookaState&& other) = delete;
		PookaState& operator=(const PookaState& other) = delete;
		PookaState& operator=(PookaState&& other) = delete;

		virtual PookaState* Update(float deltaT) = 0;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;
	};

	class SearchingState final : public PookaState
	{
	public:
		SearchingState(PookaComponent* pPooka);
		~SearchingState() = default;

		virtual PookaState* Update(float dt) override;
		virtual void OnEnter() override;
		virtual void OnExit() override {};

		void SetDir(glm::vec2 dir) { m_Dir = dir; };
	private:
		glm::vec2 m_Dir{1.f, 0.f};
		float m_MoveSpeed{ 55.f };
		const float m_MaxSearchTime{ 10.f };
		float m_CurrentSeachTime{ 0.f };
		std::vector<dae::ColliderComponent*> m_pColliders{};
		dae::ColliderComponent* m_pOwnerCollider{};

		glm::vec2 DirectionChecks(float dt, glm::vec2 prevDir);
	};

	class GhostState final : public PookaState
	{
	public:
		GhostState(PookaComponent* pPooka);
		~GhostState() = default;

		virtual PookaState* Update(float dt) override;
		virtual void OnEnter() override;
		virtual void OnExit() override {};

	private:
		enum class Axis
		{
			X,
			Y
		};

		float m_MoveSpeed{ 20.f };
		bool m_StartedGhosting{ false };
		std::vector<dae::ColliderComponent*> m_pColliders{};
		dae::ColliderComponent* m_pOwnerCollider{};
		std::vector<dae::GameObject*> m_pPlayers{};

		glm::vec2 FindClosestPlayerPos();
		bool CheckClearInAxis(Axis axis);
	};

	class StunnedState final : public PookaState
	{
	public:
		StunnedState(PookaComponent* pPooka);
		~StunnedState() = default;

		virtual PookaState* Update(float dt) override;
		virtual void OnEnter() override;
		virtual void OnExit() override {};

		void AddStretchAmount(float amount);

	private:
		float m_AirAmount{ 0.f };
		const float m_MaxAirAmount{ 100.f };
		float m_TimeSinceLastAdd{ 0.f };
		const float m_MaxTimeSinceLastAdd{ 0.5f };
		void RemoveStretch(float dt);
	};

	class PookaComponent : public game::EnemyComponent
	{
	public:
		PookaComponent(dae::GameObject* pOwner);
		virtual ~PookaComponent() = default;

		PookaComponent(const PookaComponent& other) = delete;
		PookaComponent(PookaComponent&& other) = delete;
		PookaComponent& operator=(const PookaComponent& other) = delete;
		PookaComponent& operator=(PookaComponent&& other) = delete;
		
		virtual void Update(float dt) override;

		SearchingState* GetPookaSearchState() const { return m_pSearchState.get(); };
		GhostState* GetPookaGhostState() const { return m_pGhostState.get(); };
		StunnedState* GetPookaStunnedState() const { return m_pStunnedState.get(); };

		PookaState* GetPreviousState() const { return m_PreviousState; };

		virtual void Reset() override;
		virtual void AddAir(float amount) override;
	private:
		std::unique_ptr<SearchingState> m_pSearchState{ nullptr };
		std::unique_ptr<GhostState> m_pGhostState{ nullptr };
		std::unique_ptr<StunnedState> m_pStunnedState{ nullptr };
		PookaState* m_CurrentState{ nullptr };
		PookaState* m_PreviousState{ nullptr };
	};

}