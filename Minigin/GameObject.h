#pragma once
#include <memory>
#include <vector>
#include "Transform.h"

namespace dae
{
	class Component;

	class GameObject final
	{
	public:
		void Update(float dt);
		void PhysicsUpdate(float dt);
		void Render() const;

		void SetPosition(float x, float y);
		void SetPosition(float x, float y, float z);
		void SetPosition(glm::vec3 pos);
		const Transform* GetTransform() const { return &m_transform; };

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

#pragma region ComponentTemplate

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			m_pComponents.emplace_back(std::make_shared<T>(this, std::forward<Args>(args)...));
			return *static_cast<T*>(m_pComponents.back().get());
		}

		template <typename T>
		void RemoveComponent()
		{
			for (auto it{ m_pComponents.begin() }; it != m_pComponents.end(); ++it)
			{
				if (dynamic_cast<T*>(it->get()))
				{
					m_pComponents.erase(it);
					break;
				}
			}
		}

		template <typename T>
		T* GetComponent() const
		{
			for (auto& component : m_pComponents)
			{
				T* CompPtr = dynamic_cast<T*>(component.get());
				if (CompPtr)
				{
					return CompPtr;
				}
			}
			return nullptr;
		}

		template <typename T>
		bool IsComponentPresent() const //Barely/Not used, just have it just in case I need it
		{
			for (auto& component : m_pComponents)
			{
				T* CompPtr = dynamic_cast<T*>(component.get());
				if (CompPtr)
				{
					return true;
				}
			}
			return false;
		}
#pragma endregion
	private:
		Transform m_transform{};
		std::vector<std::shared_ptr<Component>> m_pComponents{};
		std::vector<std::shared_ptr<Component>> m_pPhysicsComponents{};
	};
}
