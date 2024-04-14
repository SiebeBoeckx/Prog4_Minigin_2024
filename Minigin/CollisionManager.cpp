#include "CollisionManager.h"
#include "Components.h"

dae::CollisionManager::CollisionManager() :
	m_pColliders(std::vector<ColliderComponent*>{})
{
}

void dae::CollisionManager::AddCollider(ColliderComponent* colliders)
{
	auto i = std::find(m_pColliders.begin(), m_pColliders.end(), colliders);
	if (i == m_pColliders.end())
	{
		m_pColliders.push_back(colliders);
	}
}

void dae::CollisionManager::RemoveCollider(ColliderComponent* colliders)
{
	auto i = std::find(m_pColliders.begin(), m_pColliders.end(), colliders);
	if (i != m_pColliders.end())
	{
		m_pColliders.erase(i);
	}
}

const std::vector<dae::ColliderComponent*>& dae::CollisionManager::GetColliders() const
{
	return m_pColliders;
}