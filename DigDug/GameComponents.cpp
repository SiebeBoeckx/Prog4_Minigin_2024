#include "GameComponents.h"

namespace game
{
#pragma region PlayerComponent
    PlayerComponent::PlayerComponent(dae::GameObject* pOwner, int playerNr, int lives)
        :Component(pOwner)
        , m_PlayerNr{ playerNr }
        , m_Lives{ lives }
    {
        m_pPlayerSubject = std::make_unique<dae::Subject<game::EventType>>();
    }

    void PlayerComponent::AddObserver(dae::IObserver<game::EventType>* obs)
    {
        m_pPlayerSubject->AddObserver(obs);
    }

    void PlayerComponent::RemoveObserver(dae::IObserver<game::EventType>* obs)
    {
        m_pPlayerSubject->RemoveObserver(obs);
    }
#pragma endregion
}