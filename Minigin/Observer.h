#pragma once
//#include "Event.h"
namespace dae
{
	template<typename T>
	class IObserver
	{
	public:
		IObserver() = default;
		virtual ~IObserver() = default;
		virtual void HandleEvent(T event) = 0;
		//virtual void OnSubjectDestroy() = 0;
		IObserver(const IObserver& other) = delete;
		IObserver(IObserver&& other) = delete;
		IObserver& operator=(const IObserver& other) = delete;
		IObserver& operator=(IObserver&& other) = delete;
	};
}