#pragma once
#include <vector>
#include <memory>

#include "Observer.h"
#include "Event.h"

namespace dae
{
    template<typename... Args>
    class Subject final {
        std::vector<Observer<Event<Args...>>*> m_observers{};
    public:
        ~Subject() {
            for (auto& observer : m_observers) {
                observer->OnSubjectDestroy();
            }
        }
        void AddObserver(Observer<Event<Args...>>* observer) {
            m_observers.push_back(observer);
        }
        void RemoveObserver(Observer<Event<Args...>>* observer) {
            auto it = std::find(m_observers.begin(), m_observers.end(), observer);
            if (it != m_observers.end()) {
                m_observers.erase(it);
            }
        }
        void Notify(const Args&... args) {
            for (auto& observer : m_observers) {
                observer->HandleEvent(args...);
            }
        }
    };
}