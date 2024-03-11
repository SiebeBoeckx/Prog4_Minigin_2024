#include <stdexcept>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include <format>
#include <numbers>
#include <chrono>
#include <iostream>
#include <algorithm>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui_plot.h"

namespace dae
{
#pragma region Texture

	void TextureComponent::Update(float)
	{
		if (m_pOwnerGlobalTransform == nullptr) //Check if the transform pointer is set
		{
			m_pOwnerGlobalTransform = GetOwner()->GetGlobalTransform(); //This will only happen the first time
		}
	}

	void TextureComponent::Render() const
	{
		if (m_texture == nullptr)
		{
			return;
		}
		const float textureWidthOffset = m_texture->GetSize().x / 2.f;
		const float textureHeightOffset = m_texture->GetSize().y / 2.f;
		if (m_pOwnerGlobalTransform == nullptr ) //Check if the transform pointer is set
		{
			Renderer::GetInstance().RenderTexture(*m_texture, -textureWidthOffset, -textureHeightOffset);
		}
		else //transform pointer set
		{
			const auto& pos = m_pOwnerGlobalTransform->GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x - textureWidthOffset, pos.y - textureHeightOffset);
		}
	}

	void TextureComponent::SetTexture(const std::string& filename)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	}

	void TextureComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
	{
		m_texture = texture;
	}
#pragma endregion
#pragma region Text

	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font)
		:Component(pOwner)
		, m_needsUpdate(true)
		, m_text(text)
		, m_font(std::move(font))
	{
	}

	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
		:Component(pOwner)
		, m_needsUpdate(true)
		, m_text(text)
		, m_font(std::move(font))
		, m_Color(color)
	{
	}

	void TextComponent::Update(float)
	{
		if (m_needsUpdate)
		{
			if (m_pOwnerTexture == nullptr) //Check if the transform pointer is set
			{
				m_pOwnerTexture = GetOwner()->GetComponent<TextureComponent>(); //This will only happen the first time
			}
			else
			{
				const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_Color);
				if (surf == nullptr)
				{
					throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
				}
				auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
				if (texture == nullptr)
				{
					throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
				}
				SDL_FreeSurface(surf);
				m_pOwnerTexture->SetTexture(std::make_shared<Texture2D>(texture));
				m_needsUpdate = false;
			}
		}
	}

	// This implementation uses the "dirty flag" pattern
	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

	void TextComponent::SetColor(Uint8 r, Uint8 g, Uint8 b)
	{
		m_Color.r = r;
		m_Color.g = g;
		m_Color.b = b;
	}

	void TextComponent::SetColor(const SDL_Color& color)
	{
		m_Color = color;
	}

#pragma endregion
#pragma region FPS

	void dae::FPSComponent::Update(float deltaT)
	{
		if (m_pOwnerText == nullptr) //Check if the text pointer is set
		{
			m_pOwnerText = GetOwner()->GetComponent<TextComponent>(); //This will only happen the first time
		}
		else
		{
			m_Delay += deltaT;
			++m_Count;
			if (m_Delay >= m_MaxTimeBetweenUpdates)
			{
				m_LastFPS = m_Count / m_MaxTimeBetweenUpdates;

				std::string printString = std::format("{:.1f} FPS", m_LastFPS);
				m_pOwnerText->SetText(printString);

				m_Count = 0;
				m_Delay -= m_MaxTimeBetweenUpdates;
			}
		}
	}

#pragma endregion
#pragma region Encircle
	void Encircle::Update(float deltaT)
	{	
		const float twoPi{ static_cast<float>(std::numbers::pi) * 2 };
		m_Angle += m_RotationsPerSec * deltaT;

		if (m_Angle >= twoPi)
		{
			m_Angle -= twoPi;
		}

		m_Offset.x = m_Distance * glm::cos(m_Angle);
		m_Offset.y = m_Distance * glm::sin(m_Angle);

		GetOwner()->SetLocalPosition(glm::vec3(m_Offset.x, m_Offset.y, 0.f));	
	}
#pragma endregion
#pragma region ImGui
	void ImGuiComponent::RenderUI() const
	{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
		// Create a simple menu
		ImGui::Begin("Clear the cache + ImGui exercise");
        ImGui::InputInt("# Samples", m_pSamples);

		// Add a button
		if (ImGui::Button("All exercises"))
		{
            //*m_pString = std::string{};
            RunExercises();
		}

        //ImGui::Text("%s", (*m_pString).c_str());

        std::vector<float> xValues{ 1,2,4,8,16,32,64,128,256,512,1024 };
        static ImU32 colors[2] = { ImColor(0, 255, 0), ImColor(255, 0, 255) };

        //Exercises
#pragma region Exercise 1
        ImGui::BeginChild("Exercise 1", ImVec2(250, 250), true);
        if (ImGui::Button("Trash the cache with int"))
        {
            //*m_pString = std::string{};
            Exercise1();
        }
        
        if (m_pExercise1Results && m_pExercise1Results->data())
        {
            // Plot the data exercise 1
            ImGui::PlotConfig conf;
            conf.values.xs = xValues.data();
            conf.values.count = static_cast<int>(xValues.size());
            conf.values.ys = m_pExercise1Results->data();
            conf.values.count = static_cast<int>(m_pExercise1Results->size());
            conf.values.color = colors[0];
            conf.scale.min = 0;
            conf.scale.max = m_pExercise1Results->data()[0] * 1.1f;
            conf.tooltip.show = true;
            conf.grid_x.show = false;
            conf.grid_y.show = true;
            conf.grid_y.size = (m_pExercise1Results->data()[0] * 1.1f) / 6;
            conf.grid_y.subticks = 2;
            conf.frame_size = ImVec2(200, 200);

            ImGui::Plot("Exercise 1", conf);
        }
        ImGui::EndChild();
        ImGui::SameLine();
#pragma endregion
#pragma region Exercise 2
        ImGui::BeginChild("Exercise 2", ImVec2(300, 700), true);
        if (ImGui::Button("Trash the cache with GameObject3D"))
        {
            //*m_pString = std::string{};
            Exercise2();
        }

        if (m_pExercise2Results && m_pExercise2Results->data())
        {
            // Plot the data exercise 2
            ImGui::PlotConfig conf;
            conf.values.xs = xValues.data();
            conf.values.count = static_cast<int>(xValues.size());
            conf.values.ys = m_pExercise2Results->data();
            conf.values.count = static_cast<int>(m_pExercise2Results->size());
            conf.values.color = colors[0];
            conf.scale.min = 0;
            conf.scale.max = m_pExercise2Results->data()[0] * 1.1f;
            conf.tooltip.show = true;
            conf.grid_x.show = false;
            conf.grid_y.show = true;
            conf.grid_y.size = (m_pExercise2Results->data()[0] * 1.1f) / 6;
            conf.grid_y.subticks = 2;
            conf.frame_size = ImVec2(200, 200);

            ImGui::Plot("Exercise 2", conf);
        }
#pragma endregion
#pragma region Exercise 2 Alt
        if (ImGui::Button("Trash the cache with GameObject3DAlt"))
        {
            //*m_pString = std::string{};
            Exercise2Alt();
        }

        if (m_pExercise2AltResults && m_pExercise2AltResults->data())
        {
            // Plot the data exercise 2 Alt
            ImGui::PlotConfig conf;
            conf.values.xs = xValues.data();
            conf.values.count = static_cast<int>(xValues.size());
            conf.values.ys = m_pExercise2AltResults->data();
            conf.values.count = static_cast<int>(m_pExercise2AltResults->size());
            conf.values.color = colors[1];
            conf.scale.min = 0;
            conf.scale.max = m_pExercise2AltResults->data()[0] * 1.1f;
            conf.tooltip.show = true;
            conf.grid_x.show = false;
            conf.grid_y.show = true;
            conf.grid_y.size = (m_pExercise2AltResults->data()[0] * 1.1f) / 6;
            conf.grid_y.subticks = 2;
            conf.frame_size = ImVec2(200, 200);

            ImGui::Plot("Exercise 2 Alt", conf);
        }
#pragma endregion
#pragma region Exercise 2 Combined
        if (m_pExercise2Results && m_pExercise2Results->data() && m_pExercise2AltResults && m_pExercise2AltResults->data())
        {
            ImGui::Text("%s", "Combined Graph:");
            static const float* y_data[] = { m_pExercise2Results->data(), m_pExercise2AltResults->data() };

            // Plot the data exercise 2 combined
            ImGui::PlotConfig conf;
            conf.values.xs = xValues.data();
            conf.values.count = static_cast<int>(xValues.size());
            conf.values.ys_list = y_data;
            conf.values.ys_count = 2;
            conf.values.colors = colors;
            conf.scale.min = 0;
            conf.scale.max = m_pExercise2Results->data()[0] * 1.1f;
            conf.tooltip.show = true;
            conf.grid_x.show = false;
            conf.grid_y.show = true;
            conf.grid_y.size = (m_pExercise2Results->data()[0] * 1.1f) / 6;
            conf.grid_y.subticks = 2;
            conf.frame_size = ImVec2(200, 200);

            ImGui::Plot("Exercise 2 Combined", conf);
        }

        ImGui::EndChild();
#pragma endregion	

        ImGui::End();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiComponent::RunExercises() const
	{
		Exercise1();
		Exercise2();
		Exercise2Alt();
	}

    void ImGuiComponent::Exercise1() const
    {
        m_pExercise1Results->clear();
        std::vector<int> objectBuffer(1'000'000, 1);

        //for (int i{ 0 }; i <= 1'000'000; ++i)
        //{
        //    objectBuffer->push_back(1);
        //}

        std::chrono::high_resolution_clock::time_point startTime{};
        std::chrono::high_resolution_clock::time_point endTime{};

        std::string string{ "Int:\n" };

        for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
        {
            std::vector<std::chrono::microseconds> durations{};
            for (int count{ 0 }; count < *m_pSamples; ++count)
            {
                startTime = std::chrono::high_resolution_clock::now();
                for (size_t i{ 0 }; i < objectBuffer.size(); i += stepsize)
                {
                    (objectBuffer)[i] *= 2;
                }
                endTime = std::chrono::high_resolution_clock::now();
                std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
                durations.push_back(duration);
            }

            //Remove lowest and highest values
            auto minmax = std::minmax_element(durations.begin(), durations.end());
            auto min_val = *minmax.first;
            auto max_val = *minmax.second;

            //Removes duplicates, unwanted behaviour
            //durations.erase(std::remove(durations.begin(), durations.end(), min_val), durations.end());
            //durations.erase(std::remove(durations.begin(), durations.end(), max_val), durations.end());

            //Only remove 1 value if duplicates exist
            auto min_iter = std::find(durations.begin(), durations.end(), min_val);
            if (min_iter != durations.end())
            {
                durations.erase(min_iter);
            }

            auto max_iter = std::find(durations.begin(), durations.end(), max_val);
            if (max_iter != durations.end())
            {
                durations.erase(max_iter);
            }

            //Get average
            long long durationSum{};
            for (auto duration : durations)
            {
                durationSum += duration.count();
            }

            auto averageDuration{ durationSum / durations.size() };

            string += "stepsize: " + std::to_string(stepsize) + ", duration: " + std::to_string(averageDuration) + " microseconds\n";
            m_pExercise1Results->push_back(static_cast<float>(averageDuration));
        }
        string += '\n';

        //*m_pString += string;
    }

    void ImGuiComponent::Exercise2() const
    {
        m_pExercise2Results->clear();
        std::vector<GameObject3D> objectBuffer(1'000'000, GameObject3D{});

        //for (int i{ 0 }; i <= 1'000'000; ++i)
        //{
        //    objectBuffer->push_back(GameObject3D{});
        //}

        std::chrono::high_resolution_clock::time_point startTime{};
        std::chrono::high_resolution_clock::time_point endTime{};

        std::string string{ "GameObject3D:\n" };

        for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
        {
            std::vector<std::chrono::microseconds> durations{};
            for (int count{ 0 }; count < *m_pSamples; ++count)
            {
                startTime = std::chrono::high_resolution_clock::now();
                for (size_t i{ 0 }; i < objectBuffer.size(); i += stepsize)
                {
                    (objectBuffer)[i].id *= 2;
                }
                endTime = std::chrono::high_resolution_clock::now();
                std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
                durations.push_back(duration);
            }

            //Remove lowest and highest values
            auto minmax = std::minmax_element(durations.begin(), durations.end());
            auto min_val = *minmax.first;
            auto max_val = *minmax.second;

            //Removes duplicates, unwanted behaviour
            //durations.erase(std::remove(durations.begin(), durations.end(), min_val), durations.end());
            //durations.erase(std::remove(durations.begin(), durations.end(), max_val), durations.end());

            //Only remove 1 value if duplicates exist
            auto min_iter = std::find(durations.begin(), durations.end(), min_val);
            if (min_iter != durations.end())
            {
                durations.erase(min_iter);
            }

            auto max_iter = std::find(durations.begin(), durations.end(), max_val);
            if (max_iter != durations.end())
            {
                durations.erase(max_iter);
            }

            //Get average
            long long durationSum{};
            for (auto duration : durations)
            {
                durationSum += duration.count();
            }

            auto averageDuration{ durationSum / durations.size() };

           string += "stepsize: " + std::to_string(stepsize) + ", duration: " + std::to_string(averageDuration) + " microseconds\n";
           m_pExercise2Results->push_back(static_cast<float>(averageDuration));
        }
        string += '\n';
        //*m_pString += string;
    }

    void ImGuiComponent::Exercise2Alt() const
    {
        m_pExercise2AltResults->clear();
        std::vector<GameObject3DAlt> objectBuffer(1'000'000, GameObject3DAlt{});

        //for (int i{ 0 }; i <= 1'000'000; ++i)
        //{
        //    objectBuffer->push_back(GameObject3DAlt{});
        //}

        std::chrono::high_resolution_clock::time_point startTime{};
        std::chrono::high_resolution_clock::time_point endTime{};

        std::string string{ "GameObject3DAlt:\n" };

        for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
        {
            std::vector<std::chrono::microseconds> durations{};
            for (int count{ 0 }; count < *m_pSamples; ++count)
            {
                startTime = std::chrono::high_resolution_clock::now();
                for (size_t i{ 0 }; i < objectBuffer.size(); i += stepsize)
                {
                    (objectBuffer)[i].id *= 2;
                }
                endTime = std::chrono::high_resolution_clock::now();
                std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
                durations.push_back(duration);
            }

            //Remove lowest and highest values
            auto minmax = std::minmax_element(durations.begin(), durations.end());
            auto min_val = *minmax.first;
            auto max_val = *minmax.second;

            //Removes duplicates, unwanted behaviour
            //durations.erase(std::remove(durations.begin(), durations.end(), min_val), durations.end());
            //durations.erase(std::remove(durations.begin(), durations.end(), max_val), durations.end());

            //Only remove 1 value if duplicates exist
            auto min_iter = std::find(durations.begin(), durations.end(), min_val);
            if (min_iter != durations.end())
            {
                durations.erase(min_iter);
            }

            auto max_iter = std::find(durations.begin(), durations.end(), max_val);
            if (max_iter != durations.end())
            {
                durations.erase(max_iter);
            }

            //Get average
            long long durationSum{};
            for (auto duration : durations)
            {
                durationSum += duration.count();
            }

            auto averageDuration{ durationSum / durations.size() };

            string += "stepsize: " + std::to_string(stepsize) + ", duration: " + std::to_string(averageDuration) + " microseconds\n";
            m_pExercise2AltResults->push_back(static_cast<float>(averageDuration));
        }
        string += '\n';
        //*m_pString += string;
    }
#pragma endregion
}