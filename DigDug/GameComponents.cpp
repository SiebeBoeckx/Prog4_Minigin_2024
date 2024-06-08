#include "GameComponents.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "Wall.h"
#include "LevelLoader.h"
#include "GameSystems.h"
#include <iostream>
#include "ServiceLocator.h"
#include "Enemies.h"

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
            game::SceneSystem::GetInstance().HandleEvent(EventType::GAME_OVER);
        }
    }
#pragma endregion
#pragma region MoveableComp
    MoveableComponent::MoveableComponent(dae::GameObject* pOwner)
        :Component(pOwner)
        ,m_pOwner(pOwner)
    {
        m_pOwnerCollider = pOwner->GetComponent<dae::ColliderComponent>();
        m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
    }

    void MoveableComponent::Update(float)
    {
        m_MoveAlreadyCalled = false;
        const glm::vec2 curPos = m_pOwner->GetWorldPosition();

        if (curPos == m_PrevPos)
        {
            dae::ServiceLocator::GetSoundSystem().PauseMusic(false);
        }
        else
        {
            m_PrevPos = curPos;
            dae::ServiceLocator::GetSoundSystem().PauseMusic(true);
        }

        if (m_TargetDir == -(m_PrevDir)) //Always allow change direction in horizontal or vertical axis
        {
            m_PrevDir = m_TargetDir;
            //Move(dt);
            return;
        }

        // Check if the player is at a grid point
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
        if (m_MoveAlreadyCalled)
        {
            return;
        }
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

            if (collision->GetTag() == "PUMP")
            {
                continue;
            }
            //if (collision->GetTag() == "EDGE")
            //{
            //    std::cout << "Edge\n";
            //}
        
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
        m_MoveAlreadyCalled = true;
        //dae::ServiceLocator::GetSoundSystem().PauseMusic(false);
    }
#pragma endregion
#pragma region PumpComp
    PumpComponent::PumpComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer)
        :Component(pOwner)
        ,m_pOwner(pOwner)
        ,m_pPlayer(pPlayer)
    {
        m_pOwnerCollider = m_pOwner->GetComponent<dae::ColliderComponent>();
        m_pOwnerTexture = m_pOwner->GetComponent<dae::TextureComponent>();
        m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
    }
    void PumpComponent::Update(float dt)
    {
        if (m_FirstUpdate) //Some enemies get added after player, this means the stored colliders aren't updated
        {   
            m_pColliders = dae::CollisionManager::GetInstance().GetColliders();   
        }

        if (!m_IsActive && !m_IsHooked)
        {
            return; 
        }
        if (m_IsHooked)
        {
            m_TimeHooked += dt;
            if (m_pHookedEnemy->GetMarkObjectForDelete())
            {
                ResetHook();
                return;
            }
            m_pHookedEnemy->GetComponent<game::EnemyComponent>()->AddAir(20.f * dt);
            //std::cout << "Time hooked: " << m_TimeHooked << '\n';

            if (m_TimeHooked >= m_MaxTimeHooked)
            {
                ResetHook();
                return;
            }
            return;
        }
        //std::cout << m_CurrentOffset << '\n';
        if (m_CurrentOffset > m_Range)
        {
            ResetHook();
            return;
        }  

        if (dae::CollisionManager::GetInstance().DidCountChange())
        {
            m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
        }

        //std::cout << m_pOwnerCollider->GetGlobalPosition().x << "\n ";
        //            << m_pOwnerCollider->GetGlobalPosition().y << ", "
        //            << m_pOwnerCollider->GetGlobalPosition().z << "\n";
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
            if (collision->GetTag() == "ENEMY")
            {
                //std::cout << "Checking enemy\n";
                if (m_pOwnerCollider->IsColliding(collision))
                {
                    //std::cout << "Hooked enemy\n";
                    m_IsHooked = true;
                    m_IsActive = false;
                    m_pHookedEnemy = collision->GetOwner();
                    m_pHookedEnemy->GetComponent<game::EnemyComponent>()->SetPlayer(m_pPlayer);
                    m_TimeHooked = 0.f;
                    break;
                }
            }
        }
        m_pOwner->Translate(m_Dir * m_MoveSpeed * dt);
        m_CurrentOffset += m_MoveSpeed * dt;
    }
    void PumpComponent::Fire(glm::vec2 direction)
    {
        if (!m_IsHooked && !m_IsActive)
        {
            m_IsActive = true;

            m_Dir = direction;

            if (direction == glm::vec2{ 1.f, 0.f })
            {
                m_pOwnerTexture->SetTexture("Resources/Sprites/PumpRight.png");
            }
            else if (direction == glm::vec2{ -1.f, 0.f })
            {
                m_pOwnerTexture->SetTexture("Resources/Sprites/PumpLeft.png");
            }
            else if (direction == glm::vec2{ 0.f, 1.f })
            {
                m_pOwnerTexture->SetTexture("Resources/Sprites/PumpDown.png");
            }
            else if (direction == glm::vec2{ 0.f, -1.f })
            {
                m_pOwnerTexture->SetTexture("Resources/Sprites/PumpUp.png");
            }
            const glm::vec2 textureSize{ m_pOwnerTexture->GetTexturePtr()->GetSize() };
            m_pOwnerCollider->SetDimensions(textureSize.x, textureSize.y);
        }
        if (m_IsHooked)
        {
            //Pumping when hooked
            m_TimeHooked = 0.f;
            m_pHookedEnemy->GetComponent<game::EnemyComponent>()->AddAir(10.f);
        }
    }
    void PumpComponent::Hold()
    {
        if (!m_IsHooked)
        {
            return;
        }
        m_TimeHooked = 0.f;
    }
    void PumpComponent::ResetHook()
    {
        m_IsActive = false;
        m_IsHooked = false;
        m_TimeHooked = 0;
        m_pHookedEnemy = nullptr;
        m_pOwner->Translate(-m_Dir * m_CurrentOffset);
        m_pOwnerTexture->SetTexture("Resources/Sprites/EmptyPump.png");
        m_CurrentOffset = 0;
        return;
    }
}
#pragma endregion