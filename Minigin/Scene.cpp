#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

GameObject* Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
	return m_objects.back().get();
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float dt)
{
	for(auto& object : m_objects)
	{
		object->Update(dt);
	}
}

void dae::Scene::PhysicsUpdate(float dt)
{
	for (auto& object : m_objects)
	{
		object->PhysicsUpdate(dt);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::RenderUI() const
{
	for (const auto& object : m_objects)
	{
		object->RenderUI();
	}
}

