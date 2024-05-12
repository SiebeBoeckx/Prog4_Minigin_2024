#pragma once
#include <glm/glm.hpp>
#include "Components.h"

namespace game
{
	class WallComponent final : public dae::Component
	{
	public:
		WallComponent(dae::GameObject* pOwner, float size);
		~WallComponent();
	private:
		const glm::vec2 m_Pos;
		const float m_Size;

		std::vector<dae::ColliderComponent*> m_Colliders;
		std::vector<bool> m_IsOpenBools;
		dae::TextureComponent* m_pTextureComp;
	};

}