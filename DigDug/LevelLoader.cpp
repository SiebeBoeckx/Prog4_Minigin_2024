#include "LevelLoader.h"
#include <rapidjson.h>
#include <document.h>
#include <filereadstream.h>
#include <iostream>
#include <error/en.h>

#include "Wall.h"
#include <InputManager.h>
#include "GameCommands.h"
#include "Displays.h"
#include "ResourceManager.h"
#include "Enemies.h"

bool game::LevelCreator::CreateLevel(const std::wstring& filePath, dae::Scene* scene, dae::Subject<game::EventType>* sceneStartSubject)
{
	std::wstring fileName = filePath;
	rapidjson::Document jsonFile;
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, fileName.c_str(), L"rb");

	if (pFile != nullptr)
	{
		fseek(pFile, 0, SEEK_END);
		const size_t size = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		char* readBuffer = new char[size];
		rapidjson::FileReadStream inputStream(pFile, readBuffer, size);
		jsonFile.ParseStream(inputStream);
		delete[] readBuffer;
		fclose(pFile);

		if (jsonFile.HasParseError())
		{
			std::cerr << "Error parsing JSON file: " << rapidjson::GetParseError_En(jsonFile.GetParseError())
				<< " at offset " << jsonFile.GetErrorOffset() << std::endl;
			return false;
		}
	}
	else
	{
		return false;
	}

	const rapidjson::Value& tileSizeVal = jsonFile["tileSize"];
	const rapidjson::Value& nrRowsVal = jsonFile["nrRows"];
	const rapidjson::Value& nrColsVal = jsonFile["nrCols"];
	const rapidjson::Value& levelLayoutVal = jsonFile["levelLayout"];
	const rapidjson::Value& objectLayoutVal = jsonFile["objectLayout"];

	int tileSize = tileSizeVal.GetInt();
	int nrRows = nrRowsVal.GetInt();
	int nrCols = nrColsVal.GetInt();

	GetInstance().m_TileSize = tileSize;

	std::vector<int>levelLayout;
	std::vector<int>objectLayout;

	for (rapidjson::SizeType i = 0; i < levelLayoutVal.Size(); ++i)
	{
		// get values in the array
		levelLayout.push_back(levelLayoutVal[i].GetInt());
	}
	for (rapidjson::SizeType i = 0; i < objectLayoutVal.Size(); ++i)
	{
		// get values in the array
		objectLayout.push_back(objectLayoutVal[i].GetInt());
	}

	// starting position of the grid for the level
	glm::vec3 startPos{ 160.f, 64.f, 0.f };

	//Outer collision frame
	auto outerCollider = std::make_unique<dae::GameObject>();
	outerCollider->SetLocalPosition(0.f, 0.f);
	const float edgeThickness{ 1.f };
	//Top wall
	auto& topEdge = outerCollider->AddComponent<dae::ColliderComponent>("EDGE");
	topEdge.SetPosition(startPos.x, startPos.y - edgeThickness);
	topEdge.SetDimensions(static_cast<float>(tileSize * nrCols), edgeThickness);
	//Bottom wall
	auto& bottomEdge = outerCollider->AddComponent<dae::ColliderComponent>("EDGE");
	bottomEdge.SetPosition(startPos.x, startPos.y + tileSize * nrRows - tileSize / 2.f + edgeThickness);
	bottomEdge.SetDimensions(static_cast<float>(tileSize * nrCols), edgeThickness);
	//Left wall
	auto& leftEdge = outerCollider->AddComponent<dae::ColliderComponent>("EDGE");
	leftEdge.SetPosition(startPos.x - edgeThickness, startPos.y);
	leftEdge.SetDimensions(edgeThickness, static_cast<float>(tileSize * nrRows));
	//Right wall
	auto& rightEdge = outerCollider->AddComponent<dae::ColliderComponent>("EDGE");
	rightEdge.SetPosition(startPos.x + tileSize * nrCols - tileSize / 2.f + edgeThickness, startPos.y);
	rightEdge.SetDimensions(edgeThickness, static_cast<float>(tileSize * nrRows));

	scene->Add(std::move(outerCollider));

	for (int i = 0; i < nrRows; ++i)
	{
		float posY = startPos.y + (i * tileSize); //* tileSize;
		//std::cout << "col = " << i << "\n";

		for (int j = 0; j < nrCols; ++j)
		{
			float posX = startPos.x + (j * tileSize); //* tileSize;

			//std::cout << "row = " << j << "\n";

			int levelInput = levelLayout[i * nrCols + j];

			switch (levelInput)
			{
			case 0:		
				break;
			default:
				GetInstance().CreateWall(scene, posX, posY, levelInput);
				break;
			}
		}

	}

	for (int i = 0; i < nrRows; ++i)
	{
		float posY = startPos.y + (i * tileSize); //* tileSize;
		//std::cout << "col = " << i << "\n";

		for (int j = 0; j < nrCols; ++j)
		{
			float posX = startPos.x + (j * tileSize); //* tileSize;

			//std::cout << "row = " << j << "\n";

			int objectInput = objectLayout[i * nrCols + j];

			switch (objectInput)
			{
			case 0:
				break;
			case 1:
				GetInstance().SpawnPlayer1(posX, posY, scene, sceneStartSubject);
				break;
			case 2:
				GetInstance().SpawnPooka(posX, posY, scene);
				break;
			default:
				break;
			}
		}

	}
	return true;
}

void game::LevelCreator::CreateWall(dae::Scene* scene, float xPos, float yPos, int type) const
{
	auto wallObject = std::make_unique<dae::GameObject>();
	auto& texComp = wallObject->AddComponent<dae::TextureComponent>();
	texComp.SetTexture("Resources/Sprites/Wall.png");
	auto& wallComp = wallObject->AddComponent<game::WallComponent>(static_cast<float>(m_TileSize));
	wallObject->SetLocalPosition(xPos, yPos);
	switch (type)
	{
	case 1:
		break;
	case 3:
		wallComp.RemoveSide(game::Directions::Top);
		wallComp.RemoveSide(game::Directions::Bottom);
		break;
	case 4:
		wallComp.RemoveSide(game::Directions::Bottom);
		break;
	case 5:
		wallComp.RemoveSide(game::Directions::Top);
		break;
	case 6:
		wallComp.RemoveSide(game::Directions::Left);
		wallComp.RemoveSide(game::Directions::Right);
		break;
	case 7:
		wallComp.RemoveSide(game::Directions::Left);
		break;
	case 8:
		wallComp.RemoveSide(game::Directions::Right);
		break;
	case 9:
		wallComp.RemoveSide(game::Directions::Top);
		wallComp.RemoveSide(game::Directions::Right);
		wallComp.RemoveSide(game::Directions::Left);
		break;
	}
	scene->Add(std::move(wallObject));
}

void game::LevelCreator::SpawnPlayer1(float xPos, float yPos, dae::Scene* scene, dae::Subject<game::EventType>* sceneStartSubject)
{
	const float size{ 16.f };
	int controllerIdx = dae::InputManager::GetInstance().AddController();
	std::unique_ptr<dae::GameObject> player = std::make_unique<dae::GameObject>();
	dae::TextureComponent* texture = &player->AddComponent<dae::TextureComponent>();
	texture->SetTexture("Resources/Sprites/Walking0.png");
	player->SetLocalPosition(xPos, yPos, 0);
	game::PlayerComponent* playerComp = &player->AddComponent<game::PlayerComponent>(m_PlayerIdx);
	playerComp->SetStartPos({ xPos, yPos });
	dae::ColliderComponent* colliderComp = &player->AddComponent<dae::ColliderComponent>("PLAYER");
	colliderComp->SetDimensions(size, size);
	game::MoveableComponent* playerMoveComp = &player->AddComponent<game::MoveableComponent>();
	//colliderComp->SetPosition(100, 400);

	auto moveUp = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ 0, -1 }, 50.f);
	auto moveUpSecondary = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ 0, -1 }, 50.f);
	auto moveDown = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ 0, 1 }, 50.f);
	auto moveDownSecondary = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ 0, 1 }, 50.f);
	auto moveRight = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ 1, 0 }, 50.f);
	auto moveRightSecondary = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ 1, 0 }, 50.f);
	auto moveLeft = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ -1, 0 }, 50.f);
	auto moveLeftSecondary = std::make_unique<game::MoveCommand>(player.get(), glm::vec2{ -1, 0 }, 50.f);

	auto addPoints = std::make_unique<game::PickupItem_Command_P1>(&game::ScoreSystem::GetInstance());
	auto loseLife = std::make_unique<game::LoseLifeCommand>(player.get());

	// UP
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadUp, std::move(moveUp), controllerIdx, dae::InputManager::KeyState::Pressed);
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, std::move(moveUpSecondary), dae::InputManager::KeyState::Pressed);
	// DOWN
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadDown, std::move(moveDown), controllerIdx, dae::InputManager::KeyState::Pressed);
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, std::move(moveDownSecondary), dae::InputManager::KeyState::Pressed);
	// RIGHT
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadRight, std::move(moveRight), controllerIdx, dae::InputManager::KeyState::Pressed);
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, std::move(moveRightSecondary), dae::InputManager::KeyState::Pressed);
	// LEFT
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::DPadLeft, std::move(moveLeft), controllerIdx, dae::InputManager::KeyState::Pressed);
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, std::move(moveLeftSecondary), dae::InputManager::KeyState::Pressed);

	// ADD
	//dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::ButtonY, std::move(addPoints), controllerIdx, dae::InputManager::KeyState::Down);
	// LOSE LIFE
	//dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::ButtonX, std::move(loseLife), controllerIdx, dae::InputManager::KeyState::Down);


	// LIVES DISPLAY PLAYER 1
	auto displayFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto livesDisplayObject = std::make_unique<dae::GameObject>();
	livesDisplayObject->AddComponent<dae::TextureComponent>();
	livesDisplayObject->AddComponent<dae::TextComponent>("Empty", std::move(displayFont));
	livesDisplayObject->AddComponent<LivesDisplayComponent>(playerComp);

	playerComp->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());
	sceneStartSubject->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());

	livesDisplayObject->SetLocalPosition(40, 100);
	scene->Add(std::move(livesDisplayObject));

	// SCORE DISPLAY PLAYER 1
	displayFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto scoreDisplayObject = std::make_unique<dae::GameObject>();
	scoreDisplayObject->AddComponent<dae::TextureComponent>();
	scoreDisplayObject->AddComponent<dae::TextComponent>("Empty", std::move(displayFont));
	scoreDisplayObject->AddComponent<ScoreDisplayComponent>(&game::ScoreSystem::GetInstance(), m_PlayerIdx);

	//playerComp->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	sceneStartSubject->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	game::ScoreSystem::GetInstance().AddObserver_P1(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());

	scoreDisplayObject->SetLocalPosition(40, 120);
	scene->Add(std::move(scoreDisplayObject));

	//PUMP
	std::unique_ptr<dae::GameObject> pump = std::make_unique<dae::GameObject>();
	texture = &pump->AddComponent<dae::TextureComponent>();
	texture->SetTexture("Resources/Sprites/EmptyPump.png");
	colliderComp = &pump->AddComponent<dae::ColliderComponent>("PUMP");
	const glm::vec2 textureSize{ texture->GetTexturePtr()->GetSize() };
	colliderComp->SetDimensions(textureSize.x, textureSize.y);
	game::PumpComponent* pumpComp = &pump->AddComponent<game::PumpComponent>();
	pumpComp->SetRange(static_cast<float>(GetInstance().m_TileSize));

	auto fire = std::make_unique<game::FireCommand>(pump.get(), playerMoveComp);
	auto fireSecondary = std::make_unique<game::FireCommand>(pump.get(), playerMoveComp);
	auto hold = std::make_unique<game::HoldCommand>(pump.get(), playerMoveComp);
	auto holdSecondary = std::make_unique<game::HoldCommand>(pump.get(), playerMoveComp);

	// SHOOT
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::ButtonA, std::move(fire), controllerIdx, dae::InputManager::KeyState::Pressed);
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_SPACE, std::move(fireSecondary), dae::InputManager::KeyState::Down);
	//HOLD
	dae::InputManager::GetInstance().AddControllerCommand(dae::XBox360Controller::Button::ButtonA, std::move(hold), controllerIdx, dae::InputManager::KeyState::Pressed);
	dae::InputManager::GetInstance().AddKeyboardCommand(SDL_SCANCODE_SPACE, std::move(holdSecondary), dae::InputManager::KeyState::Pressed);

	pump->SetParent(player.get());
	scene->Add(std::move(pump));

	scene->Add(std::move(player));
	++m_PlayerIdx;
}

void game::LevelCreator::SpawnPooka(float xPos, float yPos, dae::Scene* scene) const
{
	//pooka
	auto pooka = std::make_unique<dae::GameObject>();
	dae::TextureComponent* texture = &pooka->AddComponent<dae::TextureComponent>();
	texture->SetTexture("Resources/Sprites/Pooka0.png");
	pooka->SetLocalPosition(xPos, yPos);
	dae::ColliderComponent* colliderComp = &pooka->AddComponent<dae::ColliderComponent>("ENEMY");
	colliderComp->SetDimensions(16.f, 16.f);
	game::PookaComponent* pookaComp = &pooka->AddComponent<game::PookaComponent>();
	pookaComp->SetStartPos({ xPos, yPos });

	scene->Add(std::move(pooka));
}
