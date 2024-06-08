#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	//class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		GameObject* Add(std::unique_ptr<GameObject> object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Update(float dt);
		void PhysicsUpdate(float dt);
		void Render() const;
		void RenderUI() const;


		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void TogglePauseScene(bool pause) { m_IsPaused = pause; };
		const bool GetPaused() const { return m_IsPaused; };

	private: 

		std::string m_name;
		std::vector <std::unique_ptr<GameObject>> m_objects{};
		bool m_IsPaused{ false };

		static unsigned int m_idCounter; 
		void DeletionUpdate();
	};

}
