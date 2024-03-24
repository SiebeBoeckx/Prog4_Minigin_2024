#pragma once
namespace dae
{
	template<typename... Args>
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void HandleEvent(Args... args) = 0;
		virtual void OnSubjectDestroy() = 0;
	};
}