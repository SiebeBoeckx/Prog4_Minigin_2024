#pragma once
#include "Wall.h"
#include "GameObject.h"

game::WallComponent::WallComponent(dae::GameObject* pOwner, float size)
	: Component(pOwner)
	, m_Pos{ pOwner->GetWorldPosition() }
	, m_Size{ size }
{
	for (int i{0}; i < 4; ++i)
	{
		const float wallThickness{ 1.f };
		m_IsOpenBools.push_back(false);
		dae::ColliderComponent* newCollider = &pOwner->AddComponent<dae::ColliderComponent>("WALL");
		switch (i)
		{
		case 0: //Bottom
			newCollider->SetPosition(0, 0);
			newCollider->SetDimensions(size, wallThickness);
			break;
		case 1: //Top
			newCollider->SetPosition(0, size - wallThickness);
			newCollider->SetDimensions(size, wallThickness);
			break;
		case 2: //Left
			newCollider->SetPosition(0, 0);
			newCollider->SetDimensions(wallThickness, size);
			break;
		case 3: //Right
			newCollider->SetPosition(size - wallThickness, 0);
			newCollider->SetDimensions(wallThickness, size);
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
