#pragma once
#include <glm/glm.hpp>
#include "Components.h"

namespace game
{
	enum class Directions
	{
		Left,
		Right,
		Bottom,
		Top
	};

	class WallComponent final : public dae::Component
	{
	public:
		WallComponent(dae::GameObject* pOwner, float size);
		~WallComponent() = default;

		virtual void Update(float) override;
		void RemoveSide(Directions direction);
		void DigWalls(const dae::ColliderComponent* playerCollider);

	private:
		const glm::vec2 m_Pos;
		const float m_Size;

		bool m_SideRemoved{ false };

		std::vector<dae::ColliderComponent*> m_Colliders;
		std::vector<bool> m_IsOpenBools;
		dae::TextureComponent* m_pTextureComp;
	};

}