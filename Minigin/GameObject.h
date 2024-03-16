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
		void RenderUI() const;

		//void SetPosition(float x, float y);
		//void SetPosition(float x, float y, float z);
		//void SetPosition(glm::vec3 pos);
		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalPosition(float x, float y, float z = 0);
		void SetPositionDirty();
		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();

		const Transform* GetLocalTransform() const { return &m_localTransform; };
		const Transform* GetGlobalTransform() const { return &m_globalTransform; };

		void MarkComponentForDelete(Component& component);

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

		void SetParent(GameObject* pParent, bool keepWorldPosition = false);
		const std::vector<GameObject*>& GetChildren() const { return m_pChildren; };

	private:
		std::vector<std::shared_ptr<Component>> m_pComponents{};

		Transform m_localTransform{};
		Transform m_globalTransform{};
		bool m_positionIsDirty{ true };
		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren{};

		void DeletionUpdate();
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);
		bool IsChild(GameObject* object);
	};
}
