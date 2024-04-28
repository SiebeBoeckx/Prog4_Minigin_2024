#pragma once
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
namespace dae
{
	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;
		SoundSystem(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem& operator=(SoundSystem&&) = delete;

		virtual void PlaySound(const std::string& fileName, int volume, int loops) = 0;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		NullSoundSystem() = default;

		void PlaySound(const std::string&, int, int) override;
	};

	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;
		SDLSoundSystem(const SDLSoundSystem&) = delete;
		SDLSoundSystem(SDLSoundSystem&&) = delete;
		SDLSoundSystem& operator=(const SDLSoundSystem&) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&&) = delete;


		void PlaySound(const std::string& fileName, int volume, int loops) override;

	private:
		enum class SoundType
		{
			Sound,
			Music //To be added later
		};

		struct SoundEvent
		{
			SoundType type;
			std::string fileName;
			int volume;
			int loops;
		};

		class SDL_MixerImpl;
		std::unique_ptr<SDL_MixerImpl> m_pSDL_MixerImpl;
		std::jthread m_Thread;
		std::queue<SoundEvent> m_EventQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_ConditionVariable;
		bool m_Quit = false;

		void SoundLoaderThread();
	};
}