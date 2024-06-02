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
		case 0: //Top
			newCollider->SetPosition(-size / 2 + size / 4, -size / 2 + size / 4);
			newCollider->SetDimensions(size, wallThickness);
			break;
		case 1: //Bottom
			newCollider->SetPosition(-size / 2 + size / 4, size - size / 4);
			newCollider->SetDimensions(size, wallThickness);
			break;
		case 2: //Left
			newCollider->SetPosition(-size / 2 + size / 4, -size / 2 + size / 4);
			newCollider->SetDimensions(wallThickness, size);
			break;
		case 3: //Right
			newCollider->SetPosition(size - size / 4, -size / 2 + size / 4);
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

void game::WallComponent::Update(float)
{
	if (!m_SideRemoved)
	{
		return;
	}
	m_SideRemoved = false;

	// Define textures for different combinations of open sides
	const std::vector<std::string> textureLookup = {
		"Resources/Sprites/Wall.png",						// 0000
		"Resources/Sprites/WallRight.png",					// 0001
		"Resources/Sprites/WallLeft.png",					// 0010
		"Resources/Sprites/WallLeftRight.png",				// 0011
		"Resources/Sprites/WallBottom.png",					// 0100
		"Resources/Sprites/WallBottomRight.png",			// 0101
		"Resources/Sprites/WallBottomLeft.png",				// 0110
		"Resources/Sprites/WallTopOnly.png",				// 0111
		"Resources/Sprites/WallTop.png",					// 1000
		"Resources/Sprites/WallTopRight.png",				// 1001
		"Resources/Sprites/WallTopLeft.png",				// 1010
		"Resources/Sprites/WallBottomOnly.png",				// 1011
		"Resources/Sprites/WallTopBottom.png",				// 1100
		"Resources/Sprites/WallLeftOnly.png",				// 1101
		"Resources/Sprites/WallRightOnly.png",				// 1110
		"Resources/Sprites/WallEmpty.png"					// 1111
	};

	// Convert the state of open or closed sides into an index for the lookup table
	int lookupIndex = (m_IsOpenBools[0] ? 8 : 0) + (m_IsOpenBools[1] ? 4 : 0) + (m_IsOpenBools[2] ? 2 : 0) + (m_IsOpenBools[3] ? 1 : 0);

	// Set the texture based on the lookup table
	m_pTextureComp->SetTexture(textureLookup[lookupIndex]);
}

void game::WallComponent::RemoveSide(Directions direction)
{
	switch (direction)
	{
	case game::Directions::Left:
		m_IsOpenBools[2] = true;
		m_Colliders[2]->MarkForDelete();
		break;
	case game::Directions::Right:
		m_IsOpenBools[3] = true;
		m_Colliders[3]->MarkForDelete();
		break;
	case game::Directions::Top:
		m_IsOpenBools[0] = true;
		m_Colliders[0]->MarkForDelete();
		break;
	case game::Directions::Bottom:
		m_IsOpenBools[1] = true;
		m_Colliders[1]->MarkForDelete();
		break;
	default:
		break;
	}

	m_SideRemoved = true;
}
