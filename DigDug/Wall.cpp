#pragma once
#include "Wall.h"
#include "GameObject.h"

game::WallComponent::WallComponent(dae::GameObject* pOwner, float size)
	: Component(pOwner)
	, m_Pos{ pOwner->GetWorldPosition() }
	, m_Size{ size }
{
	for (int i{}; i < 4; ++i)
	{
		const float wallThickness{ 1.f };
		m_IsOpenBools.push_back(false);
		dae::ColliderComponent* newCollider = &pOwner->AddComponent<dae::ColliderComponent>("WALL");
		switch (i)
		{
		case 0: //Bottom
			newCollider->SetDimensions(size, wallThickness);
			break;
		case 1: //Top
			newCollider->SetPosition(0, size - wallThickness);
			newCollider->SetDimensions(size, wallThickness);
			break;
		}
		m_Colliders.push_back(newCollider);
	}

	m_pTextureComp = pOwner->GetComponent<dae::TextureComponent>();
	m_pTextureComp->SetTexture("Resources/Sprites/Wall.png");
}

game::WallComponent::~WallComponent()
{
}
