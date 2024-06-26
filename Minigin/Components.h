#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <SDL_ttf.h>
#include <string>
//#include "GameObject.h"
#include "Texture2D.h"
#include "Font.h"
#include "Subject.h"

namespace dae
{
	class GameObject;
	class Transform;

	class Component
	{
	public:
		//Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;


		virtual void BeginPlay() {};
		virtual void Update(float) {};
		virtual void FixedUpdate(float) {};
		virtual void Render() const {};
		virtual void RenderUI() const {};
		virtual void SetPosition(float, float, float) {};
		virtual void SetPosition(glm::vec3) {};

		GameObject* GetOwner() const { return m_pOwner; };
		void MarkForDelete() { m_MarkForDelete = true; };
		//const bool GetMarkForDelete() const { return m_MarkForDelete; };
		const bool GetInValid() const;

	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {};
	private:
		GameObject* m_pOwner{ nullptr };
		bool m_MarkForDelete{ false };
	};

	class TextureComponent final : public Component
	{
	public:
		TextureComponent(GameObject* pOwner)
			:Component(pOwner)
		{};

		void Update(float) override;
		void Render() const override;
		void SetTexture(const std::string& filename);
		void SetTexture(std::unique_ptr<Texture2D> texture);
		const Texture2D* GetTexturePtr() const { return m_texture.get(); };
	private:
		std::unique_ptr<Texture2D> m_texture{ nullptr };
		const Transform* m_pOwnerGlobalTransform{ nullptr };
	};

	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font> font);
		TextComponent(GameObject* pOwner, const std::string& text, std::unique_ptr<Font> font, const SDL_Color& color);

		void Update(float) override;

		void SetText(const std::string& text);
		void SetColor(Uint8 r, Uint8 g, Uint8 b);
		void SetColor(const SDL_Color& color);
	private:
		bool m_needsUpdate; //Only update at initialize or if text changes
		std::string m_text;
		std::unique_ptr<Font> m_font{ nullptr };
		SDL_Color m_Color{ 255,255,255 };
		TextureComponent* m_pOwnerTexture{ nullptr };
	};

	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner)
			:Component(pOwner)
		{};

		void Update(float deltaT) override;
		float GetNrOfFrames() const { return m_LastFPS; };

	private:
		int m_Count{ 0 };
		const float m_MaxTimeBetweenUpdates{ 0.3f };
		float m_Delay{ 0.f };
		float m_LastFPS{ 0.f };
		TextComponent* m_pOwnerText{ nullptr };
	};

	class Encircle final : public Component
	{
	public:
		Encircle(GameObject* pOwner, float distance = 0.f, float rotationsPerSec = 1.f)
			:Component(pOwner)
			, m_Distance(distance)
			, m_RotationsPerSec(rotationsPerSec)
		{};

		void Update(float deltaT) override;
	private:
		glm::vec3 m_Offset{};
		float m_Distance{};
		float m_RotationsPerSec{};
		float m_Angle{};
		bool m_isRotatingClockwise{};
	};

	class ImGuiComponent final : public Component
	{
	public:
		ImGuiComponent(GameObject* pOwner)
			:Component(pOwner)
			//,m_pString{new std::string{}}
			, m_pExercise1Results{new std::vector<float>{}}
			, m_pExercise2Results{new std::vector<float>{}}
			, m_pExercise2AltResults{new std::vector<float>{}}
		{};

		~ImGuiComponent() override
		{
			//delete m_pString;

			delete m_pSamples;
			delete m_pExercise2AltResults;
			delete m_pExercise2Results;
			delete m_pExercise1Results;
		} 

		void RenderUI() const override;

	private:
		struct transform
		{
			float matrix[16] = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};

		class GameObject3D
		{
		public:
			transform local;
			int id;
		};

		class GameObject3DAlt
		{
		public:
			transform* local;
			int id;
		};

		void RunExercises() const;

		void Exercise1() const;
		void Exercise2() const;
		void Exercise2Alt() const;

		//std::string* m_pString{};

		std::vector<float>* m_pExercise1Results{};
		std::vector<float>* m_pExercise2Results{};
		std::vector<float>* m_pExercise2AltResults{};
		int* m_pSamples{ new int{ 100 } };
	};

	class ColliderComponent final : public Component
	{
	public:
		struct Collider
		{
			float xMin, yMin, width, height;
		};

		ColliderComponent(GameObject* go, std::string tag);
		~ColliderComponent() override;

		bool IsColliding(const ColliderComponent* otherCollider) const;
		void Update(float) override;
		void SetDimensions(float width, float height);
		void SetPosition(float xPos, float yPos);
		const Collider GetColliderBox() const { return m_ColliderBox; }
		const glm::vec3 GetGlobalPosition() const { return { m_ColliderBox.xMin, m_ColliderBox.yMin, 0 }; }
		const glm::vec2 GetLocalPosition() const { return { m_LocalPos.x, m_LocalPos.y }; }
		const std::string GetTag() const { return m_Tag; };

	private:
		Collider m_ColliderBox{};
		std::string m_Tag{ "ALL" };
		const Transform* m_pOwnerGlobalTransform{ nullptr };
		glm::vec3 m_LocalPos{};
	};
}