#pragma once
#include <string>
#include "Singleton.h"
#include "GameObject.h"
#include "Scene.h"
#include "Event.h"

namespace game
{
	class LevelCreator : public dae::Singleton<LevelCreator>
	{
	public:
		static bool CreateLevel(const std::wstring& filePath, dae::Scene* scene, dae::Subject<game::EventType>* sceneStartSubject);
		const int GetTileSize() const { return m_TileSize; };

	private:
		void CreateWall(dae::Scene* scene, float xPos, float yPos, int type) const;
		void SpawnPlayer1(float xPos, float yPos, dae::Scene* scene, dae::Subject<game::EventType>* sceneStartSubject);
		void SpawnPooka(float xPos, float yPos, dae::Scene* scene) const;

		int m_TileSize{ 0 };
		int m_PlayerIdx{ 0 };
	};
}