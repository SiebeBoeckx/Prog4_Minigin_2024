#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float dt)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(dt);
	}
}

void dae::SceneManager::PhysicsUpdate(float dt)
{
	for (auto& scene : m_scenes)
	{
		scene->PhysicsUpdate(dt);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::RenderUI()
{
	for (const auto& scene : m_scenes)
	{
		scene->RenderUI();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	m_scenes.emplace_back(std::make_unique<Scene>(name));
	return *m_scenes.back();
}
