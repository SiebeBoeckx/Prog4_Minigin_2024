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
		void Update();
		void AddCollider(ColliderComponent* collider);
		void RemoveCollider(ColliderComponent* collider);
		const std::vector<ColliderComponent*>& GetColliders() const;
		const bool DidCountChange() const { return m_CountChanged; };

	private:
		std::vector<ColliderComponent*> m_pColliders;
		bool m_CountChanged{ false };
	};
}