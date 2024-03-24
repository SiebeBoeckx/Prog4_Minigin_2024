#pragma once
#include "Event.h"
namespace dae
{
	//template<typename... Args>
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;
		virtual void HandleEvent(EventType event) = 0;
		//virtual void OnSubjectDestroy() = 0;
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;
	};
}