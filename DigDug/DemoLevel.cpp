#include "DemoLevel.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameCommands.h"
#include "Displays.h"
#include "Wall.h"

DemoLevel::DemoLevel(dae::Scene* pScene)
{
	std::unique_ptr<dae::Subject<game::EventType>> sceneStartSubject = std::make_unique<dae::Subject<game::EventType>>();


	auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 18);
	auto fpsObject = std::make_unique<dae::GameObject>();
	fpsObject->AddComponent<dae::TextureComponent>();
	fpsObject->AddComponent<dae::TextComponent>("Empty", std::move(fpsFont));
	fpsObject->AddComponent<dae::FPSComponent>();
	fpsObject->SetLocalPosition(40, 10);
	pScene->Add(std::move(fpsObject));

	//===============================================================================================================
	//2 PLAYERS (1 KEYBOARD, 1 CONTROLLER)
	//===============================================================================================================
	auto font = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto player1ControlsText = std::make_unique<dae::GameObject>();
	player1ControlsText->AddComponent<dae::TextureComponent>();
	player1ControlsText->AddComponent<dae::TextComponent>("Use D-Pad to move Dig-Dug, Y to add points, X to lose lives", std::move(font));
	player1ControlsText->SetLocalPosition(220, 60);
	pScene->Add(std::move(player1ControlsText));

	font = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto player2ControlsText = std::make_unique<dae::GameObject>();
	player2ControlsText->AddComponent<dae::TextureComponent>();
	player2ControlsText->AddComponent<dae::TextComponent>("Use WASD to move Pooka, P to add points, L to lose lives", std::move(font));
	player2ControlsText->SetLocalPosition(208, 80);
	pScene->Add(std::move(player2ControlsText));

	//player 1
	int playerIdx{ 0 };
	int controllerIdx = dae::InputManager::GetInstance().AddController();
	auto player1 = std::make_unique<dae::GameObject>();
	dae::TextureComponent* texture = &player1->AddComponent<dae::TextureComponent>();
	texture->SetTexture("Resources/Sprites/Walking0.png");
	player1.get()->SetLocalPosition(100, 400);
	game::PlayerComponent* playerComp = &player1->AddComponent<game::PlayerComponent>(playerIdx);
	dae::ColliderComponent* colliderComp = &player1->AddComponent<dae::ColliderComponent>("PLAYER");
	colliderComp->SetDimensions(15.2f, 15.2f);
	//colliderComp->SetPosition(100, 400);

	auto moveUp = std::make_unique<game::MoveCommand>(player1.get(), glm::vec2{ 0, -1 }, 50.f);
	auto moveDown = std::make_unique<game::MoveCommand>(player1.get(), glm::vec2{ 0, 1 }, 50.f);
	auto moveRight = std::make_unique<game::MoveCommand>(player1.get(), glm::vec2{ 1, 0 }, 50.f);
	auto moveLeft = std::make_unique<game::MoveCommand>(player1.get(), glm::vec2{ -1, 0 }, 50.f);

	auto addPoints = std::make_unique<game::PickupItem_Command_P1>(&game::ScoreSystem::GetInstance());
	auto loseLife = std::make_unique<game::LoseLifeCommand>(player1.get());

	// UP
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadUp, std::move(moveUp), controllerIdx, dae::InputManager::KeyState::Pressed);
	// DOWN
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadDown, std::move(moveDown), controllerIdx, dae::InputManager::KeyState::Pressed);
	// RIGHT
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadRight, std::move(moveRight), controllerIdx, dae::InputManager::KeyState::Pressed);
	// LEFT
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadLeft, std::move(moveLeft), controllerIdx, dae::InputManager::KeyState::Pressed);

	// ADD
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::ButtonY, std::move(addPoints), controllerIdx, dae::InputManager::KeyState::Down);
	// LOSE LIFE
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::ButtonX, std::move(loseLife), controllerIdx, dae::InputManager::KeyState::Down);

	pScene->Add(std::move(player1));

	// LIVES DISPLAY PLAYER 1
	auto displayFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto livesDisplayObject = std::make_unique<dae::GameObject>();
	livesDisplayObject->AddComponent<dae::TextureComponent>();
	livesDisplayObject->AddComponent<dae::TextComponent>("Empty", std::move(displayFont));
	livesDisplayObject->AddComponent<LivesDisplayComponent>(playerComp);

	playerComp->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());
	sceneStartSubject->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());

	livesDisplayObject->SetLocalPosition(40, 100);
	pScene->Add(std::move(livesDisplayObject));

	// SCORE DISPLAY PLAYER 1
	displayFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto scoreDisplayObject = std::make_unique<dae::GameObject>();
	scoreDisplayObject->AddComponent<dae::TextureComponent>();
	scoreDisplayObject->AddComponent<dae::TextComponent>("Empty", std::move(displayFont));
	scoreDisplayObject->AddComponent<ScoreDisplayComponent>(&game::ScoreSystem::GetInstance(), playerIdx);

	//playerComp->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	sceneStartSubject->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	game::ScoreSystem::GetInstance().AddObserver_P1(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());

	scoreDisplayObject->SetLocalPosition(40, 120);
	pScene->Add(std::move(scoreDisplayObject));


	//player 2
	++playerIdx;
	auto player2 = std::make_unique<dae::GameObject>();
	texture = &player2->AddComponent<dae::TextureComponent>();
	texture->SetTexture("Resources/Sprites/Pooka0.png");
	player2.get()->SetLocalPosition(50, 400);
	playerComp = &player2->AddComponent<game::PlayerComponent>(playerIdx);
	colliderComp = &player2->AddComponent<dae::ColliderComponent>("ENEMY");
	colliderComp->SetDimensions(15.2f, 15.2f);
	//colliderComp->SetPosition(50, 400);

	moveUp = std::make_unique<game::MoveCommand>(player2.get(), glm::vec2{ 0, -1 }, 100.f);
	moveDown = std::make_unique<game::MoveCommand>(player2.get(), glm::vec2{ 0, 1 }, 100.f);
	moveRight = std::make_unique<game::MoveCommand>(player2.get(), glm::vec2{ 1, 0 }, 100.f);
	moveLeft = std::make_unique<game::MoveCommand>(player2.get(), glm::vec2{ -1, 0 }, 100.f);

	auto addPoints2 = std::make_unique<game::PickupItem_Command_P2>(&game::ScoreSystem::GetInstance());
	loseLife = std::make_unique<game::LoseLifeCommand>(player2.get());

	// UP
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, std::move(moveUp), dae::InputManager::KeyState::Pressed);
	// DOWN
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, std::move(moveDown), dae::InputManager::KeyState::Pressed);
	// RIGHT
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, std::move(moveRight), dae::InputManager::KeyState::Pressed);
	// LEFT
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, std::move(moveLeft), dae::InputManager::KeyState::Pressed);

	// ADD POINTS
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_P, std::move(addPoints2), dae::InputManager::KeyState::Down);
	// LOSE LIFE
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_L, std::move(loseLife), dae::InputManager::KeyState::Down);

	pScene->Add(std::move(player2));

	// LIVES DISPLAY PLAYER 2
	displayFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	livesDisplayObject = std::make_unique<dae::GameObject>();
	livesDisplayObject->AddComponent<dae::TextureComponent>();
	livesDisplayObject->AddComponent<dae::TextComponent>("Empty", std::move(displayFont));
	livesDisplayObject->AddComponent<LivesDisplayComponent>(playerComp);

	playerComp->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());
	sceneStartSubject->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());

	livesDisplayObject->SetLocalPosition(40, 140);
	pScene->Add(std::move(livesDisplayObject));

	// SCORE DISPLAY PLAYER 2
	displayFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	scoreDisplayObject = std::make_unique<dae::GameObject>();
	scoreDisplayObject->AddComponent<dae::TextureComponent>();
	scoreDisplayObject->AddComponent<dae::TextComponent>("Empty", std::move(displayFont));
	scoreDisplayObject->AddComponent<ScoreDisplayComponent>(&game::ScoreSystem::GetInstance(), playerIdx);

	//playerComp->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	sceneStartSubject->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	game::ScoreSystem::GetInstance().AddObserver_P2(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());

	scoreDisplayObject->SetLocalPosition(40, 160);
	pScene->Add(std::move(scoreDisplayObject));

	//TEST WALL
	auto wallObject = std::make_unique<dae::GameObject>();
	wallObject->AddComponent<dae::TextureComponent>();
	wallObject->AddComponent<game::WallComponent>(16.f);
	wallObject->SetLocalPosition(300, 400);
	pScene->Add(std::move(wallObject));

	// START GAME
	sceneStartSubject->Notify(game::EventType::START_GAME);
}