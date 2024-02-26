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
			glm::vec3 pos{ m_transform.GetPosition() };
			component->SetPosition(pos.x, pos.y, pos.z);
			component->Update(dt);
		}

		//Delete marked components
		for (auto it{ m_pComponents.begin() }; it != m_pComponents.end(); ++it)
		{
			if (it->get()->GetMarkForDelete())
			{
				m_pComponents.erase(it);
				break;
			}
		}

	}

	void GameObject::PhysicsUpdate(float dt)
	{
		for (const auto& component : m_pPhysicsComponents)
		{
			glm::vec3 pos{ m_transform.GetPosition() };
			component->SetPosition(pos.x, pos.y, pos.z);
			component->FixedUpdate(dt);
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
