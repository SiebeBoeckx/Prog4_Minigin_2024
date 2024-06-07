#include "Enemies.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include <random>

// Define the static members
//game::SearchingState game::PookaState::m_Searching(nullptr);
//game::GhostState game::PookaState::m_Ghost(nullptr);     


glm::vec2 ChooseRandomDirection(glm::vec2 prevDir);

game::PookaState::PookaState(PookaComponent* pPooka)
	:m_pPooka{ pPooka }
{
}

game::SearchingState::SearchingState(PookaComponent* pPooka)
	:PookaState(pPooka)
{
	m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
    m_pOwnerCollider = pPooka->GetOwner()->GetComponent<dae::ColliderComponent>();
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
}

glm::vec2 game::SearchingState::DirectionChecks(float dt, glm::vec2 prevDir)
{
    if (dae::CollisionManager::GetInstance().DidCountChange())
    {
        m_pColliders = dae::CollisionManager::GetInstance().GetColliders();
    }

    const glm::vec2 curPos = m_pOwnerCollider->GetLocalPosition();

    glm::vec2 newPos = curPos + glm::vec2{ prevDir * m_MoveSpeed * dt };

    m_pOwnerCollider->SetPosition(newPos.x, newPos.y);
    m_pOwnerCollider->Update(0.f);

    for (auto collision : m_pColliders)
    {
        //skipping same collision
        if (collision == m_pOwnerCollider)
        {
            continue;
        }
        if (m_pOwnerCollider->IsColliding(collision))
        {
            //std::cout << "Colliding\n";
            // if is colliding with something after an update, don't update movement!
            m_pOwnerCollider->SetPosition(curPos.x, curPos.y);

            // Choose a new random direction
            glm::vec2 newDir = ChooseRandomDirection(prevDir);

            return DirectionChecks(dt, newDir);
        }
    }
    return glm::vec2(prevDir);
}

glm::vec2 ChooseRandomDirection(glm::vec2 prevDir)
{
    std::vector<glm::vec2> directions = {
        glm::vec2{1, 0}, glm::vec2{-1, 0},
        glm::vec2{0, 1}, glm::vec2{0, -1}
    };

    // Remove the previous direction from the list to avoid going back immediately
    directions.erase(std::remove(directions.begin(), directions.end(), prevDir), directions.end());

    // Generate a random index
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(directions.size()) - 1);

    return directions[dis(gen)];
}

game::GhostState::GhostState(PookaComponent* pPooka)
	:PookaState(pPooka)
{
}

game::PookaState* game::GhostState::Update(float)
{
    return m_pPooka->GetPookaGhostState();
}

void game::GhostState::OnEnter()
{
}

game::PookaComponent::PookaComponent(dae::GameObject* pOwner, float size)
	: Component(pOwner)
	, m_Size{ size }
	, m_pSearchState{ std::make_unique<SearchingState>(this) }
	, m_pGhostState{ std::make_unique<GhostState>(this) }
{
	m_pCollider = &pOwner->AddComponent<dae::ColliderComponent>("ENEMY");
	m_pCollider->SetDimensions(size, size);
    m_CurrentState = m_pSearchState.get();
}

void game::PookaComponent::Update(float dt)
{
    PookaState* newState = m_CurrentState->Update(dt);

    if (newState != m_CurrentState)
    {
        newState->OnEnter();
    }

    m_CurrentState = newState;
}
