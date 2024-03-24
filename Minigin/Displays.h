#pragma once
#include "Components.h"
#include "Observer.h"

class LivesDisplayComponent final : public dae::Component, public dae::Observer
{
public:
	LivesDisplayComponent(dae::GameObject* pOwner, dae::PlayerComponent* player);

	//~LivesDisplayComponent() override;

	LivesDisplayComponent(const LivesDisplayComponent& other) = delete;
	LivesDisplayComponent(LivesDisplayComponent&& other) noexcept = delete;
	LivesDisplayComponent& operator=(const LivesDisplayComponent& other) = delete;
	LivesDisplayComponent& operator=(LivesDisplayComponent&& other) noexcept = delete;

	void HandleEvent(EventType event) override;

private:
	dae::TextComponent* m_pTextComponent;
	std::string m_Text;
	dae::PlayerComponent* m_pPlayer{};
};

class ScoreDisplayComponent final : public dae::Component, public dae::Observer
{
public:
	ScoreDisplayComponent(dae::GameObject* pOwner, dae::PlayerComponent* player);

	//~ScoreDisplayComponent() override;

	ScoreDisplayComponent(const ScoreDisplayComponent& other) = delete;
	ScoreDisplayComponent(ScoreDisplayComponent&& other) noexcept = delete;
	ScoreDisplayComponent& operator=(const ScoreDisplayComponent& other) = delete;
	ScoreDisplayComponent& operator=(ScoreDisplayComponent&& other) noexcept = delete;

	void HandleEvent(EventType event) override;

private:
	dae::TextComponent* pTextComponent;
	std::string m_Text;
	dae::PlayerComponent* m_pPlayer{};
	int m_Score{ 0 };
};

