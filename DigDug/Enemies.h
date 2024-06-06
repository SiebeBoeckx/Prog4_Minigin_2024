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

		virtual void Update(float deltaT) = 0;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

		static SearchingState m_Searching;
		static GhostState m_Ghost;
	};

	class SearchingState final : public PookaState
	{
	public:
		SearchingState(PookaComponent* pPooka);
		~SearchingState() = default;

		virtual void Update(float dt) override;
		virtual void OnEnter() override;
		virtual void OnExit() override {};
	private:
		glm::vec2 m_PrevDir{0.f, 0.f};
		float m_MoveSpeed{ 60.f };
		glm::vec2 m_ClosestPlayerPos{ 0.f, 0.f };
	};

	class GhostState final : public PookaState
	{
	public:
		GhostState(PookaComponent* pPooka);
		~GhostState() = default;

		virtual void Update(float) override;
		virtual void OnEnter() override;
		virtual void OnExit() override {};
	};

	class PookaComponent : public dae::Component
	{
	public:
		PookaComponent(dae::GameObject* pOwner, float size);
		~PookaComponent() = default;

		PookaComponent(const PookaComponent& other) = delete;
		PookaComponent(PookaComponent&& other) = delete;
		PookaComponent& operator=(const PookaComponent& other) = delete;
		PookaComponent& operator=(PookaComponent&& other) = delete;
	private:
		dae::ColliderComponent* m_pCollider{ nullptr };
		const float m_Size;
		std::unique_ptr<PookaState> m_pState{ nullptr };
	};

}