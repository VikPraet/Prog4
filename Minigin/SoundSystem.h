#pragma once
#include <memory>
#include <string>

class ISoundService
{
public:
    virtual ~ISoundService() = default;
    virtual void PlaySound(const std::string& file) = 0;
    virtual void PlayMusic(const std::string& file) = 0;
    virtual void StopMusic() = 0;
    virtual void SetSoundVolume(int volume) = 0;
    virtual void SetMusicVolume(int volume) = 0;
};

class NullSoundService final : public ISoundService
{
public:
    void PlaySound(const std::string&) override { }
    void PlayMusic(const std::string&) override { }
    void StopMusic() override { }
    void SetSoundVolume(int) override { }
    void SetMusicVolume(int) override { }
};

class SoundService final : public ISoundService
{
public:
    SoundService();
    ~SoundService() override;

    void PlaySound(const std::string& file) override;
    void PlayMusic(const std::string& file) override;
    void StopMusic() override;
    void SetSoundVolume(int volume) override;
    void SetMusicVolume(int volume) override;

private:
    class Impl;
    std::unique_ptr<Impl> m_PImpl;
};
