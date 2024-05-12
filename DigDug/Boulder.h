#pragma once
#include "Components.h"

class BoulderComponent; // Forward declaration
class IdleState;

class BoulderState
{
protected:
	BoulderComponent* m_pBoulder{ nullptr };
public:
	BoulderState(BoulderComponent* pBoulder);
	~BoulderState() = default;

	BoulderState(const BoulderState& other) = delete;
	BoulderState(BoulderState&& other) = delete;
	BoulderState& operator=(const BoulderState& other) = delete;
	BoulderState& operator=(BoulderState&& other) = delete;

	virtual void Update(float deltaT) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	static IdleState m_Idle;
};

class IdleState final : public BoulderState
{
public:
	IdleState(BoulderComponent* pBoulder);
	~IdleState() = default;

	virtual void Update(float) override;
	virtual void OnEnter() override {};
	virtual void OnExit() override {};
};


class BoulderComponent final : public dae::Component
{
public:
	BoulderComponent(dae::GameObject* pOwner, float size);
	~BoulderComponent() = default;

	BoulderComponent(const BoulderComponent& other) = delete;
	BoulderComponent(BoulderComponent&& other) = delete;
	BoulderComponent& operator=(const BoulderComponent& other) = delete;
	BoulderComponent& operator=(BoulderComponent&& other) = delete;
private:
	dae::ColliderComponent* m_pCollider{ nullptr };
	const float m_Size;
};