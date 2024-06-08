#include "Enemies.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include <random>
#include "LevelLoader.h"
#include "GameCommands.h"

// Define the static members
//game::SearchingState game::PookaState::m_Searching(nullptr);
//game::GhostState game::PookaState::m_Ghost(nullptr);     


glm::vec2 ChooseRandomDirection(std::vector<glm::vec2> prevDirs);

game::PookaState::PookaState(PookaComponent* pPooka)
	:m_pPooka{ pPooka }
{
}

game::SearchingState::SearchingState(PookaComponent* pPooka)
	:PookaState(pPooka)
{
	m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
    m_pOwnerCollider = m_pPooka->GetCollider();
}

game::PookaState* game::SearchingState::Update(float dt)
{
	m_CurrentSeachTime += dt;
	if (m_CurrentSeachTime >= m_MaxSearchTime)
	{
		return m_pPooka->GetPookaGhostState();
	}

    m_Dir = DirectionChecks(dt, m_Dir);
    m_pPooka->GetOwner()->Translate(m_Dir * m_MoveSpeed * dt);
	return m_pPooka->GetPookaSearchState();
}

void game::SearchingState::OnEnter()
{
	m_CurrentSeachTime = 0.f;
    m_pPooka->GetOwner()->GetComponent<dae::TextureComponent>()->SetTexture("Resources/Sprites/Pooka0.png");
}

glm::vec2 game::SearchingState::DirectionChecks(float dt, glm::vec2 prevDir)
{
    //Check if it's at a gridPoint, change of axis allowed
    const glm::vec2 curOwnerPos = m_pPooka->GetOwner()->GetWorldPosition();
    const int gridSize = game::LevelCreator::GetInstance().GetTileSize();

    const float epsilon = 1.f;
    bool atGridPoint =  (std::fabs(std::fmod(curOwnerPos.x, gridSize)) < epsilon) &&
                        (std::fabs(std::fmod(curOwnerPos.y, gridSize)) < epsilon);

    if (!atGridPoint)
    {
        return prevDir;
    }
    
    std::vector<glm::vec2> prevDirs{};
    bool checkedStraight{ false };
    while (true)
    {
        //Here the ai will try to change axis
        // Choose a new random direction
        glm::vec2 newDir{};
        if (checkedStraight)
        {
            newDir = ChooseRandomDirection(prevDirs);
        }
        else
        {
            newDir = prevDir;
            checkedStraight = true;
        }
        // If no more directions are available, return the previous direction
        //if (newDir == glm::vec2{ 0, 0 })
        //{
        //    return prevDir;
        //}

        //Check collision
        if (dae::CollisionManager::GetInstance().DidCountChange())
        {
            m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
        }

        const glm::vec2 curPos = m_pOwnerCollider->GetLocalPosition();
        const glm::vec2 newPos = curPos + glm::vec2{ newDir * static_cast<float>(gridSize / 4.f) };

        m_pOwnerCollider->SetPosition(newPos.x, newPos.y);
        m_pOwnerCollider->Update(0.f);

        bool collisionDetected = false;
        for (auto& collision : m_pColliders)
        {
            if (collision->GetInValid())
            {
                continue;
            }
            // Skipping same collision
            if (collision == m_pOwnerCollider)
            {
                continue;
            }
            // Hit player(s)
            if (collision->GetTag() == "PLAYER")
            {
                glm::vec2 nextFramePos = curPos + glm::vec2{ newDir * m_MoveSpeed * dt };

                m_pOwnerCollider->SetPosition(nextFramePos.x, nextFramePos.y);
                m_pOwnerCollider->Update(0.f);

                if (m_pOwnerCollider->IsColliding(collision))
                {
                    auto loseLife = std::make_unique<game::LoseLifeCommand>(collision->GetOwner());
                    loseLife->Execute(0.f);
                }
                continue;
            }
            if (m_pOwnerCollider->IsColliding(collision))
            {
                // If colliding with something after an update, don't update movement!
                m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
                collisionDetected = true;
                break;
            }
        }

        if (!collisionDetected)
        {
            m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
            return newDir;
        }

        // Add the current direction to the list of tried directions
        prevDirs.push_back(newDir);
    }   
}

glm::vec2 ChooseRandomDirection(std::vector<glm::vec2> prevDirs)
{
    std::vector<glm::vec2> directions = {
        glm::vec2{1, 0}, glm::vec2{-1, 0},
        glm::vec2{0, 1}, glm::vec2{0, -1}
    };

    // Remove previously tried directions from the list
    for (const auto& dir : prevDirs)
    {
        directions.erase(std::remove(directions.begin(), directions.end(), dir), directions.end());
    }

    // If no directions are left, return a default direction (this case should be handled properly in the main logic)
    if (directions.empty())
    {
        return glm::vec2{ 0, 0 }; // Or handle this case differently
    }

    // Generate a random index
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(directions.size()) - 1);

    return directions[dis(gen)];
}

game::GhostState::GhostState(PookaComponent* pPooka)
	:PookaState(pPooka)
{
    m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
    m_pOwnerCollider = m_pPooka->GetCollider();

    for (auto& collision : m_pColliders)
    {
        if (collision->GetInValid())
        {
            continue;
        }
        if (collision->GetTag() == "PLAYER")
        {
            m_pPlayers.push_back(collision->GetOwner());
        }
    }
}

game::PookaState* game::GhostState::Update(float dt)
{
    //Check collision
    if (dae::CollisionManager::GetInstance().DidCountChange())
    {
        m_pColliders = dae::CollisionManager::GetInstance().GetColliders();

        m_pPlayers.clear();
        //Refill players
        for (auto& collision : m_pColliders)
        {
            if (collision->GetInValid())
            {
                continue;
            }
            if (collision->GetTag() == "PLAYER")
            {
                m_pPlayers.push_back(collision->GetOwner());
            }
        }
    }
    const glm::vec2 ownPos = m_pPooka->GetOwner()->GetWorldPosition();
    const int gridSize = game::LevelCreator::GetInstance().GetTileSize();

    const float epsilon = 0.5f;
    const bool atGridX = (std::fabs(std::fmod(ownPos.x, gridSize)) < epsilon);
    const bool atGridY = (std::fabs(std::fmod(ownPos.y, gridSize)) < epsilon);

    const glm::vec2 targetDir = glm::normalize(FindClosestPlayerPos() - ownPos);

    const glm::vec2 curPos = m_pOwnerCollider->GetLocalPosition();
    const glm::vec2 newPos = curPos + glm::vec2{ targetDir * m_MoveSpeed * dt };

    m_pOwnerCollider->SetPosition(newPos.x, newPos.y);
    m_pOwnerCollider->Update(0.f);

    bool isColliding = false;
    for (auto& collision : m_pColliders)
    {
        if (collision->GetInValid())
        {
            continue;
        }
        // Skipping same collision
        if (collision == m_pOwnerCollider)
        {
            continue;
        }
        // Hit player(s)
        if (collision->GetTag() == "PLAYER")
        {
            if (m_pOwnerCollider->IsColliding(collision))
            {
                auto loseLife = std::make_unique<game::LoseLifeCommand>(collision->GetOwner());
                loseLife->Execute(0.f);
            }
            continue;
        }
        if (m_pOwnerCollider->IsColliding(collision))
        {
            // Now it has started going through walls
            if (!m_StartedGhosting)
            {
                m_StartedGhosting = true;
                break;
            }
            isColliding = true;
            break;
        }
    }

    // Calculate the closest valid grid point for x-coordinate
    float gridX = (std::round(ownPos.x / gridSize) * gridSize);

    // Calculate the closest valid grid point for y-coordinate
    float gridY = (std::round(ownPos.y / gridSize) * gridSize);

    glm::vec2 translationVector{ 0.f, 0.f };

    if (m_StartedGhosting)
    {
        if (isColliding)
        {
            m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
            m_pPooka->GetOwner()->Translate(targetDir * m_MoveSpeed * dt);
            return m_pPooka->GetPookaGhostState();
        }

        if (atGridX)
        {
            if (CheckClearInAxis(Axis::Y))
            {
                translationVector.x = gridX - ownPos.x;
                m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
                m_pPooka->GetOwner()->Translate(translationVector);
                m_pPooka->GetPookaSearchState()->SetDir({ 0.f, 1.f });
                return m_pPooka->GetPookaSearchState();
            }
        }
        if (atGridY)
        {
            if (CheckClearInAxis(Axis::X))
            {
                translationVector.y = gridY - ownPos.y;
                m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
                m_pPooka->GetOwner()->Translate(translationVector);
                m_pPooka->GetPookaSearchState()->SetDir({ 1.f, 0.f });
                return m_pPooka->GetPookaSearchState();
            }
        }
    }
    m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
    m_pPooka->GetOwner()->Translate(targetDir * m_MoveSpeed * dt);
    return m_pPooka->GetPookaGhostState();
}

void game::GhostState::OnEnter()
{
    m_StartedGhosting = false;
    m_pPooka->GetOwner()->GetComponent<dae::TextureComponent>()->SetTexture("Resources/Sprites/PookaGhost.png");
}

glm::vec2 game::GhostState::FindClosestPlayerPos()
{
    float shortestDistance{ FLT_MAX };
    glm::vec2 closestPos{};
    const glm::vec2 ownPos = m_pPooka->GetOwner()->GetWorldPosition();

    for (auto& player : m_pPlayers)
    {
        const glm::vec2 playerPos = player->GetWorldPosition();
        const float distance = glm::distance(ownPos, playerPos);
        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            closestPos = playerPos;
        }
    }
    return closestPos;
}

bool game::GhostState::CheckClearInAxis(Axis axis)
{
    for (int i{}; i < 2; ++i)
    {
        glm::vec2 targetDir{0.f, 0.f};
        //Select direction
        switch (axis)
        {
        case game::GhostState::Axis::X:
            if (i)
            {
                targetDir.x = 1;
            }
            else
            {
                targetDir.x = -1;
            }
            break;
        case game::GhostState::Axis::Y:
            if (i)
            {
                targetDir.y = 1;
            }
            else
            {
                targetDir.y = -1;
            }
            break;
        default:
            break;
        }

        const int gridSize = game::LevelCreator::GetInstance().GetTileSize();

        const glm::vec2 curPos = m_pOwnerCollider->GetLocalPosition();
        const glm::vec2 newPos = curPos + glm::vec2{ targetDir * static_cast<float>(gridSize / 4.f) };

        m_pOwnerCollider->SetPosition(newPos.x, newPos.y);
        m_pOwnerCollider->Update(0.f);

        for (auto& collision : m_pColliders)
        {
            if (collision->GetInValid())
            {
                continue;
            }
            // Skipping same collision
            if (collision == m_pOwnerCollider)
            {
                continue;
            }
            // Skip player(s)
            if (collision->GetTag() == "PLAYER")
            {
                continue;
            }
            if (m_pOwnerCollider->IsColliding(collision))
            {
                // Now it has started going through walls
                m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
                return false;
            }
        }
        m_pOwnerCollider->SetPosition(curPos.x, curPos.y);
    }
    return true;
}

game::PookaComponent::PookaComponent(dae::GameObject* pOwner)
	: Component(pOwner)
{
    m_pCollider = pOwner->GetComponent<dae::ColliderComponent>();

    m_pSearchState = std::make_unique<SearchingState>(this);
    m_pGhostState = std::make_unique<GhostState>(this);
    m_CurrentState = m_pSearchState.get();
}

void game::PookaComponent::Update(float dt)
{
    PookaState* newState = m_CurrentState->Update(dt);

    if (newState != m_CurrentState)
    {
        m_CurrentState->OnExit();
        newState->OnEnter();
    }

    m_CurrentState = newState;
}
