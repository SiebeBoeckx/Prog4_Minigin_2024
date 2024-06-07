#pragma once
#include "Components.h"

namespace game
{
	class PookaComponent; // Forward declaration
	class SearchingState;
	class GhostState;

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
	private:
		glm::vec2 m_Dir{1.f, 0.f};
		float m_MoveSpeed{ 60.f };
		glm::vec2 m_ClosestPlayerPos{ 0.f, 0.f };
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

		virtual PookaState* Update(float) override;
		virtual void OnEnter() override;
		virtual void OnExit() override {};
	};

	class PookaComponent : public dae::Component
	{
	public:
		PookaComponent(dae::GameObject* pOwner, float size);
		virtual ~PookaComponent() = default;

		PookaComponent(const PookaComponent& other) = delete;
		PookaComponent(PookaComponent&& other) = delete;
		PookaComponent& operator=(const PookaComponent& other) = delete;
		PookaComponent& operator=(PookaComponent&& other) = delete;
		
		virtual void Update(float dt) override;

		PookaState* GetPookaSearchState() const { return m_pSearchState.get(); };
		PookaState* GetPookaGhostState() const { return m_pGhostState.get(); };
	private:
		dae::ColliderComponent* m_pCollider{ nullptr };
		const float m_Size;
		std::unique_ptr<SearchingState> m_pSearchState{ nullptr };
		std::unique_ptr<GhostState> m_pGhostState{ nullptr };
		PookaState* m_CurrentState{ nullptr };
	};

}