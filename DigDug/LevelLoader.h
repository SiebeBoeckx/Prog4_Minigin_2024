#pragma once
#include <string>
#include "Singleton.h"
#include "GameObject.h"
#include "Scene.h"

namespace game
{
	class LevelCreator : public dae::Singleton<LevelCreator>
	{
	public:
		static bool CreateLevel(const std::wstring& filePath, dae::Scene* scene);
		const dae::GameObject* GetPlayer1() const { return m_pPlayer1; };
		void SetPlayer1(dae::GameObject* player1) { m_pPlayer1 = player1; };
		const int GetTileSize() const { return m_TileSize; };

	private:
		void CreateWall(dae::Scene* scene, float xPos, float yPos, int type) const;
		void SpawnPlayer(float xPos, float yPos) const;

		dae::GameObject* m_pPlayer1 = nullptr;
		int m_TileSize{ 0 };
	};
}