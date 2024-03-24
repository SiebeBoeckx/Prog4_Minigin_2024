#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

namespace dae
{
	GameObject::~GameObject() = default;

	void GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
	{
		if (IsChild(pParent) || pParent == this || m_pParent == pParent)
		{
			return;
		}

		if (pParent == nullptr)
		{
			SetLocalPosition(GetWorldPosition());
		}
		else
		{
			if (keepWorldPosition)
			{
				SetLocalPosition(GetWorldPosition() - pParent->GetWorldPosition()); //Position set dirty in SetLocalPosition()
			}
			else
			{
				SetPositionDirty();
			}
		}

		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		m_pParent = pParent;
		if (m_pParent)
		{
			m_pParent->AddChild(this);
		}
	}

	void GameObject::Translate(glm::vec2 vector)
	{
		SetLocalPosition(m_localTransform.GetPosition() + glm::vec3{ vector.x, vector.y, 0 });
	}

	void GameObject::DeletionUpdate()
	{
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

	void GameObject::AddChild(GameObject* child)
	{
		m_pChildren.push_back(child);
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		const auto childToDelete = std::find(m_pChildren.begin(), m_pChildren.end(), child);
		if (childToDelete != m_pChildren.end())
		{
			m_pChildren.erase(childToDelete);
		}
	}

	bool GameObject::IsChild(GameObject* object)
	{
		const auto childToFind = std::find(m_pChildren.begin(), m_pChildren.end(), object);
		if (childToFind != m_pChildren.end())
		{
			return true;
		}
		return false;
	}

	void GameObject::Update(float dt)
	{
		for (const auto& component : m_pComponents)
		{
			glm::vec3 pos{ GetWorldPosition() };
			//component->SetPosition(pos.x, pos.y, pos.z);
			component->Update(dt);
		}

		DeletionUpdate();
	}

	void GameObject::PhysicsUpdate(float dt)
	{
		for (const auto& component : m_pComponents)
		{
			glm::vec3 pos{ GetWorldPosition() };
			//component->SetPosition(pos.x, pos.y, pos.z);
			component->FixedUpdate(dt);
		}

		DeletionUpdate();
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_pComponents)
		{
			component->Render();
		}
	}

	void GameObject::RenderUI() const
	{
		for (const auto& component : m_pComponents)
		{
			component->RenderUI();
		}
	}

	void GameObject::SetLocalPosition(const glm::vec3& pos)
	{
		m_localTransform.SetPosition(pos);
		SetPositionDirty();
	}

	void GameObject::SetLocalPosition(float x, float y, float z)
	{
		m_localTransform.SetPosition(x, y, z);
		SetPositionDirty();
	}

	void GameObject::SetPositionDirty()
	{
		if (m_positionIsDirty) //No need if already set
		{
			return;
		}
		m_positionIsDirty = true;
		for (const auto& child : m_pChildren)
		{
			child->SetPositionDirty();
		}
	}

	const glm::vec3& GameObject::GetLocalPosition() const
	{
		return m_localTransform.GetPosition();
	}

	const glm::vec3& GameObject::GetWorldPosition()
	{
		if (m_positionIsDirty)
		{
			UpdateWorldPosition();
		}
		return m_globalTransform.GetPosition();
	}

	void GameObject::UpdateWorldPosition()
	{
		if (m_positionIsDirty)
		{
			if (m_pParent == nullptr)
			{
				m_globalTransform.SetPosition(m_localTransform.GetPosition());
			}
			else
			{
				m_globalTransform.SetPosition(m_pParent->GetWorldPosition() + m_localTransform.GetPosition());
			}
		}
		m_positionIsDirty = false;
	}

	//void GameObject::SetPosition(float x, float y)
	//{
	//	m_transform.SetPosition(x, y, 0.0f);
	//}
	//
	//void GameObject::SetPosition(float x, float y, float z)
	//{
	//	m_transform.SetPosition(x, y, z);
	//}
	//
	//void GameObject::SetPosition(glm::vec3 pos)
	//{
	//	m_transform.SetPosition(pos);
	//}

	void GameObject::MarkComponentForDelete(Component& component)
	{
		component.MarkForDelete();
	}
}
