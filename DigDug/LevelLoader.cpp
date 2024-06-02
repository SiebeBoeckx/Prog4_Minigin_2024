#include "LevelLoader.h"
#include <rapidjson.h>
#include <document.h>
#include <filereadstream.h>
#include <iostream>
#include <error/en.h>

#include "Wall.h"

bool game::LevelCreator::CreateLevel(const std::wstring& filePath, dae::Scene* scene)
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

	int tileSize = tileSizeVal.GetInt();
	int nrRows = nrRowsVal.GetInt();
	int nrCols = nrColsVal.GetInt();

	GetInstance().m_TileSize = tileSize;

	std::vector<int>levelLayOut;

	for (rapidjson::SizeType i = 0; i < levelLayoutVal.Size(); ++i)
	{
		// get values in the array
		levelLayOut.push_back(levelLayoutVal[i].GetInt());
	}

	// starting position of the grid for the level
	glm::vec3 startPos{ 144.f, 64.f, 0.f };


	for (int i = 0; i < nrRows; ++i)
	{
		float posY = startPos.y + (i * tileSize); //* tileSize;
		//std::cout << "col = " << i << "\n";

		for (int j = 0; j < nrCols; ++j)
		{
			float posX = startPos.x + (j * tileSize); //* tileSize;

			//std::cout << "row = " << j << "\n";

			int input = levelLayOut[i * nrCols + j];

			switch (input)
			{
			case 0:		
				break;
			case 2:
				GetInstance().SpawnPlayer(posX, posY);
				break;
			default:
				GetInstance().CreateWall(scene, posX, posY, input);
				break;
			}
		}

	}
	return true;
}

void game::LevelCreator::CreateWall(dae::Scene* scene, float xPos, float yPos, int type) const
{
	auto wallObject = std::make_unique<dae::GameObject>();
	wallObject->AddComponent<dae::TextureComponent>();
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
	}
	scene->Add(std::move(wallObject));
}

void game::LevelCreator::SpawnPlayer(float xPos, float yPos) const
{
	m_pPlayer1->SetLocalPosition(xPos, yPos, 0);
}
