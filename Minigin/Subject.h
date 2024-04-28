#pragma once
#include <vector>
#include <memory>

#include "Observer.h"

namespace dae
{
	template<typename T>
	class Subject final {
		std::vector<IObserver<T>*> m_observers{};
	public:
		~Subject() = default;
		//{
		//	for (auto& observer : m_observers)
		//	{
		//		observer->OnSubjectDestroy();
		//	}
		//}
		void AddObserver(IObserver<T>* observer)
		{
			m_observers.push_back(observer);
		}
		void RemoveObserver(IObserver<T>* observer)
		{
			m_observers.erase(std::remove(
				m_observers.begin(),
				m_observers.end(), observer),
				m_observers.end());
		}
		void Notify(T event)
		{
			for (auto& observer : m_observers)
			{
				observer->HandleEvent(event);
			}
		}
	};
}