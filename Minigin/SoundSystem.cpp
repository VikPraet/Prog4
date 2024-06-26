#include "SoundSystem.h"
#include <SDL_mixer.h>
#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <unordered_map>
#include <mutex>

class SoundService::Impl
{
public:
    Impl()
    {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }

        if (Mix_AllocateChannels(128) == -1)
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }

    ~Impl()
    {
        Mix_CloseAudio();
    }

    void PlaySound(const std::string& file)
    {
        auto future = std::async(std::launch::async, [file, this]()
            {
	            const std::string fullPath = "../Data/" + file;
	            if (Mix_Chunk* sound = Mix_LoadWAV(fullPath.c_str()))
                {
	                const int channel = Mix_PlayChannel(-1, sound, 0);
                    if (channel == -1)
                    {
                        std::cerr << "Mix_PlayChannel: " << Mix_GetError() << std::endl;
                        Mix_FreeChunk(sound);
                    }
                    else
                    {
                        std::lock_guard lock(m_Mutex);
                        Mix_Volume(channel, m_SoundVolume);
                        m_Sounds[channel] = sound;
                    }
                }
                else
                {
                    std::cerr << "Unable to load sound file: " << file << " SDL_mixer Error: " << Mix_GetError() << std::endl;
                }
            });
        m_Futures.push_back(std::move(future));
    }

    void PlayMusic(const std::string& file)
    {
        auto future = std::async(std::launch::async, [file, this]()
            {
                const std::string fullPath = "../Data/" + file;
                if (Mix_Music* music = Mix_LoadMUS(fullPath.c_str()))
                {
                    Mix_PlayMusic(music, -1);
                    Mix_VolumeMusic(m_MusicVolume);
                }
                else
                {
                    std::cerr << "Unable to load music file: " << file << " SDL_mixer Error: " << Mix_GetError() << std::endl;
                }
            });
        m_Futures.push_back(std::move(future));
    }

    void StopMusic()
    {
        auto future = std::async(std::launch::async, []()
            {
                Mix_HaltMusic();
            });
        m_Futures.push_back(std::move(future));
    }

    void SetSoundVolume(int volume)
    {
        m_SoundVolume = volume;
    }

    void SetMusicVolume(int volume)
    {
        m_MusicVolume = volume;
        Mix_VolumeMusic(m_MusicVolume);
    }

    int GetSoundVolume()
    {
        return m_SoundVolume;
    }

    int GetMusicVolume()
    {
	    return m_MusicVolume;
    }

private:
    std::vector<std::future<void>> m_Futures;
    std::unordered_map<int, Mix_Chunk*> m_Sounds; // Map from channel to chunk
    std::mutex m_Mutex; // Mutex to protect the map
    int m_SoundVolume = MIX_MAX_VOLUME;
    int m_MusicVolume = MIX_MAX_VOLUME;
};

SoundService::SoundService() : m_PImpl(std::make_unique<Impl>()) { }

SoundService::~SoundService() = default;

void SoundService::PlaySoundEffect(const std::string& file)
{
    m_PImpl->PlaySound(file);
}

void SoundService::PlayMusic(const std::string& file)
{
    m_PImpl->PlayMusic(file);
}

void SoundService::StopMusic()
{
    m_PImpl->StopMusic();
}

void SoundService::SetSoundVolume(int volume)
{
    m_PImpl->SetSoundVolume(volume);
}

void SoundService::SetMusicVolume(int volume)
{
    m_PImpl->SetMusicVolume(volume);
}

int SoundService::GetSoundVolume()
{
    return m_PImpl->GetSoundVolume();
}

int SoundService::GetMusicVolume()
{
    return m_PImpl->GetMusicVolume();
}
