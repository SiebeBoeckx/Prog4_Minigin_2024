#pragma once
#include <vector>
#include <memory>

#include "Observer.h"

namespace dae
{
	//template<typename... Args>
	class Subject final {
		std::vector<IObserver*> m_observers{};
	public:
		~Subject() = default;
		//{
		//	for (auto& observer : m_observers)
		//	{
		//		observer->OnSubjectDestroy();
		//	}
		//}
		void AddObserver(IObserver* observer)
		{
			m_observers.push_back(observer);
		}
		void RemoveObserver(IObserver* observer)
		{
			m_observers.erase(std::remove(
				m_observers.begin(),
				m_observers.end(), observer),
				m_observers.end());
		}
		void Notify(EventType event)
		{
			for (auto& observer : m_observers)
			{
				observer->HandleEvent(event);
			}
		}
	};
}