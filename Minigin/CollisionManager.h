#pragma once
#include <vector>
#include "Singleton.h"

// Singleton manager for all colliders

namespace dae
{
	class ColliderComponent;

	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		CollisionManager();
		~CollisionManager() = default;
		void AddCollider(ColliderComponent* collider);
		void RemoveCollider(ColliderComponent* collider);
		const std::vector<ColliderComponent*>& GetColliders() const;

	private:
		std::vector<ColliderComponent*> m_pColliders;
	};
}