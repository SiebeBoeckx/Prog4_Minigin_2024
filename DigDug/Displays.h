#pragma once
#include "GameComponents.h"
//#include "Observer.h"
#include "GameSystems.h"

class LivesDisplayComponent final : public dae::Component, public dae::IObserver<game::EventType>
{
public:
	LivesDisplayComponent(dae::GameObject* pOwner, game::PlayerComponent* player);

	//~LivesDisplayComponent() override;

	LivesDisplayComponent(const LivesDisplayComponent& other) = delete;
	LivesDisplayComponent(LivesDisplayComponent&& other) noexcept = delete;
	LivesDisplayComponent& operator=(const LivesDisplayComponent& other) = delete;
	LivesDisplayComponent& operator=(LivesDisplayComponent&& other) noexcept = delete;

	void HandleEvent(game::EventType event) override;

private:
	dae::TextComponent* m_pTextComponent;
	std::string m_Text;
	game::PlayerComponent* m_pPlayer{};
};

class ScoreDisplayComponent final : public dae::Component, public dae::IObserver<game::EventType>
{
public:
	ScoreDisplayComponent(dae::GameObject* pOwner, game::ScoreSystem* score, int idx);

	//~ScoreDisplayComponent() override;

	ScoreDisplayComponent(const ScoreDisplayComponent& other) = delete;
	ScoreDisplayComponent(ScoreDisplayComponent&& other) noexcept = delete;
	ScoreDisplayComponent& operator=(const ScoreDisplayComponent& other) = delete;
	ScoreDisplayComponent& operator=(ScoreDisplayComponent&& other) noexcept = delete;

	void HandleEvent(game::EventType event) override;

private:
	void UpdateScoreText();

	dae::TextComponent* pTextComponent;
	std::string m_Text;
	game::ScoreSystem* m_pScore{};
	int m_Idx{};
};

