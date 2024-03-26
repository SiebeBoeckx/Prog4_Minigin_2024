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
		const bool GetMarkForDelete() const { return m_MarkForDelete; };

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

	class PlayerComponent final : public Component
	{
	public:
		PlayerComponent(dae::GameObject* pOwner, int playerNr, int lives = 3);

		void AddObserver(IObserver* obs);
		void RemoveObserver(IObserver* obs);
		int GetLives() const { return m_Lives; }
		int GetPoints() const { return m_Points; }

		void AddPoints(int amount)
		{
			m_Points += amount;
			m_pPlayerSubject->Notify(EventType::ADD_POINTS);
		}

		void LoseLife()
		{
			--m_Lives;
			m_pPlayerSubject->Notify(EventType::PLAYER_DIED);
			if (m_Lives <= 0)
			{
				m_pPlayerSubject->Notify(EventType::GAME_OVER);
			}
		}
	private:
		const int m_PlayerNr;
		int m_Lives;
		int m_Points;

		std::unique_ptr<Subject> m_pPlayerSubject{};
	};
}