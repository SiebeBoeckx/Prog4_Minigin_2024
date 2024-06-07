#include "GameComponents.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "Wall.h"
#include "LevelLoader.h"
#include "GameSystems.h"
#include <iostream>

namespace game
{
#pragma region PlayerComponent
    PlayerComponent::PlayerComponent(dae::GameObject* pOwner, int playerNr, int lives)
        :Component(pOwner)
        , m_PlayerNr{ playerNr }
        , m_Lives{ lives }
    {
        m_pPlayerSubject = std::make_unique<dae::Subject<game::EventType>>();
        m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
    }

    void PlayerComponent::Update(float)
    {
        auto* playerCollider = GetOwner()->GetComponent<dae::ColliderComponent>();
        if (dae::CollisionManager::GetInstance().DidCountChange())
        {
            m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
        }

        for (auto& collision : m_pColliders)
        {
            if (collision->GetInValid())
            {
                continue;
            }
            if (!collision->IsColliding(playerCollider))
            {
                continue;
            }
            if (collision->GetTag() == "WALL")
            {
                game::WallComponent* wallComp = collision->GetOwner()->GetComponent<game::WallComponent>();
                wallComp->DigWalls(playerCollider);
                switch (m_PlayerNr)
                {
                case 0:
                    game::ScoreSystem::GetInstance().HandleEvent(game::DIG_P1);
                    break;
                case 1:
                    game::ScoreSystem::GetInstance().HandleEvent(game::DIG_P2);
                    break;
                default:
                    break;
                }
            }
        }
    }

    void PlayerComponent::AddObserver(dae::IObserver<game::EventType>* obs)
    {
        m_pPlayerSubject->AddObserver(obs);
    }

    void PlayerComponent::RemoveObserver(dae::IObserver<game::EventType>* obs)
    {
        m_pPlayerSubject->RemoveObserver(obs);
    }
    void PlayerComponent::LoseLife()
    {
        GetOwner()->SetLocalPosition(glm::vec3{ m_StartPos, 0 });
        --m_Lives;
        m_pPlayerSubject->Notify(EventType::PLAYER_DIED);
        if (m_Lives <= 0)
        {
            m_pPlayerSubject->Notify(EventType::GAME_OVER);
        }
    }
#pragma endregion
#pragma region MoveableComp
    MoveableComponent::MoveableComponent(dae::GameObject* pOwner)
        :Component(pOwner)
        ,m_pOwner(pOwner)
    {
        m_pOwnerCollider = pOwner->GetComponent<dae::ColliderComponent>();
    }

    void MoveableComponent::Update(float)
    {
        if (m_TargetDir == -(m_PrevDir)) //Always allow change direction in horizontal or vertical axis
        {
            m_PrevDir = m_TargetDir;
            //Move(dt);
            return;
        }

        // Check if the player is at a grid point
        const glm::vec2 curPos = m_pOwner->GetWorldPosition();
        const int gridSize = LevelCreator::GetInstance().GetTileSize();

        const float epsilon = 1.f;
        bool atGridPoint =  (std::fabs(std::fmod(curPos.x, gridSize)) < epsilon) &&
                            (std::fabs(std::fmod(curPos.y, gridSize)) < epsilon);

        if (atGridPoint)
        {
            //std::cout << "At grid point\n";
            m_PrevDir = m_TargetDir;
        }
        //Move(dt);
    }
    void MoveableComponent::Move(float dt)
    {
        if (dae::CollisionManager::GetInstance().DidCountChange())
        {
        	m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
        }

        const glm::vec2 curPos = m_pOwnerCollider->GetLocalPosition();

        glm::vec2 newPos = curPos + glm::vec2{ m_PrevDir * m_MoveSpeed * dt };
        
        m_pOwnerCollider->SetPosition(newPos.x, newPos.y);
        m_pOwnerCollider->Update(0.f);
        
        for (auto& collision : m_pColliders)
        {
            if (collision->GetInValid())
            {
                continue;
            }
        	//skipping same collision
        	if (collision == m_pOwnerCollider)
        	{
        		continue;
        	}

            if (collision->GetTag() == "WALL")
            {
                continue;
            }
        
        	if (m_pOwnerCollider->IsColliding(collision))
        	{
        		//std::cout << "Colliding\n";
        		// if is colliding with something after an update, don't update movement!
        		m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
        		return;
        		
        	}
        }
        //std::cout << curPos.x << ", " << curPos.y << '\n';
        //std::cout << m_pOwner->GetGlobalTransform()->GetPosition().x << ", " << m_pOwner->GetGlobalTransform()->GetPosition().y << ", " << m_pOwner->GetGlobalTransform()->GetPosition().z << '\n';
        m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
        m_pOwner->Translate(m_PrevDir * m_MoveSpeed * dt);
    }
}
#pragma endregion