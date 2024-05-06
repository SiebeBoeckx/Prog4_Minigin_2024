#include "SoundSystem.h"
#include <iostream>
#include <map>
#include <SDL_mixer.h>
#include <SDL.h>


namespace dae
{
    void NullSoundSystem::PlaySound(const std::string&, int, int)
    {
        std::cout << "Calling PlaySound on a NullSoundSystem\n";
    }
#pragma region SDL Sound System
    class SDLSoundSystem::SDL_MixerImpl final
    {
    public:
        SDL_MixerImpl()
        {
            Mix_Init(MIX_INIT_MP3);
            SDL_Init(SDL_INIT_AUDIO);
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                printf("SDL_Mixer couldnt init. Err: %s\n", Mix_GetError());
        }
        ~SDL_MixerImpl()
        {
            for (auto& file : m_SoundsMap)
            {
                Mix_FreeChunk(file.second);
            }
            Mix_Quit();
        }
        SDL_MixerImpl(const SDL_MixerImpl& other) = delete;
        SDL_MixerImpl operator=(const SDL_MixerImpl& rhs) = delete;
        SDL_MixerImpl(SDL_MixerImpl&& other) = delete;
        SDL_MixerImpl operator=(SDL_MixerImpl&& rhs) = delete;

        void PlaySound(const std::string& fileName, int volume, int loops)
        {
            // Load file if not loaded already
            {
                std::unique_lock lock(m_MapMutex);

                if (!m_SoundsMap.contains(fileName))
                {
                    // Error, can't load file
                    if (!LoadSound(fileName))
                    {
                        return;
                    }
                }
            }
            // Volume == percentage
            volume = (MIX_MAX_VOLUME * volume) / 100;

            // Play the sound
            Mix_VolumeChunk(m_SoundsMap[fileName], volume);
            Mix_PlayChannel(1, m_SoundsMap[fileName], loops);
        }

    private:
        bool LoadSound(const std::string& fileName)
        {
            Mix_Chunk* mixChunk = Mix_LoadWAV(fileName.c_str());
            if (mixChunk == nullptr)
            {
                printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
                return false;
            }
            m_SoundsMap.insert(std::make_pair(fileName, mixChunk));
            return true;
        }

        std::map<std::string, Mix_Chunk*> m_SoundsMap;
        std::mutex m_MapMutex;
    };

    SDLSoundSystem::SDLSoundSystem()
        : SoundSystem()
    {
        m_pSDL_MixerImpl = std::make_unique<SDL_MixerImpl>();
        m_Thread = std::jthread(&SDLSoundSystem::SoundLoaderThread, this);
    }

    SDLSoundSystem::~SDLSoundSystem()
    {
        {
            std::lock_guard lock(m_QueueMutex);
            m_Quit = true;
        }
        m_ConditionVariable.notify_all();
    }

    void SDLSoundSystem::PlaySound(const std::string& fileName, int volume, int loops)
    {
        {
            std::lock_guard lock(m_QueueMutex);
            m_EventQueue.push({ SoundType::Sound, fileName, volume, loops });
            //make sure the lock gets unlocked before notifying
        }
        m_ConditionVariable.notify_all();
    }

    void SDLSoundSystem::SoundLoaderThread()
    {
        while (true)
        {
            //Acquire a lock
            std::unique_lock lock(m_QueueMutex);

            //This unlocks the lock and makes the thread sleep untill it gets notified
            //If notified, it checks the lambda to see if it can continue
            //If it can continue it locks the lock again
            m_ConditionVariable.wait(lock, [this] { return !m_EventQueue.empty() || m_Quit; });

            if (m_Quit)
            {
                break;
            }

            SoundEvent event = m_EventQueue.front();
            m_EventQueue.pop();
            lock.unlock();
            //Unlock here, no need to lock the queue after popping the sound to be loaded/played
            if (event.type == SoundType::Sound)
            {
                m_pSDL_MixerImpl->PlaySound(event.fileName, event.volume, event.loops);
            }
        }
    }
#pragma endregion
}