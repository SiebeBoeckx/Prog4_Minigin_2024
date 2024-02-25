#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Components.h"

namespace dae
{
	GameObject::~GameObject() = default;

	void GameObject::Update(float dt)
	{
		for (const auto& component : m_pComponents)
		{
			component->Update(dt);
			glm::vec3 pos{ m_transform.GetPosition() };
			component->SetPosition(pos.x, pos.y, pos.z);
		}
	}

	void GameObject::PhysicsUpdate(float dt)
	{
		for (const auto& component : m_pPhysicsComponents)
		{
			component->Update(dt);
			glm::vec3 pos{ m_transform.GetPosition() };
			component->SetPosition(pos.x, pos.y, pos.z);
		}
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_pComponents)
		{
			component->Render();
		}
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.0f);
	}

	void GameObject::SetPosition(float x, float y, float z)
	{
		m_transform.SetPosition(x, y, z);
	}

	void GameObject::SetPosition(glm::vec3 pos)
	{
		m_transform.SetPosition(pos);
	}
}
