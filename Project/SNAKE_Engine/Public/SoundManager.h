#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

#include "../ThirdParty/fmod/inc/fmod.hpp"

class SNAKE_Engine;
using SoundInstanceID = uint64_t;

class SoundManager
{
    friend  SNAKE_Engine;
public:

    void LoadSound(const std::string& tag, const std::string& filepath, bool loop = false);
    SoundInstanceID Play(const std::string& tag, float volume = 1.0f, float startTimeSec = 0.0f);

    void SetVolumeByID(SoundInstanceID id, float volume);
    void SetVolumeByTag(const std::string& tag, float volume);
    void SetVolumeAll(float volume);

    enum class SoundControlType { Pause, Resume, Stop};
    void ControlByID(SoundControlType control, SoundInstanceID id);
    void ControlByTag(SoundControlType control, const std::string& tag);
    void ControlAll(SoundControlType control);



private:
    SoundManager();
    void Init();
    void Update();
    void Cleanup();
    void Free();

    FMOD::System* system;
    std::unordered_map<std::string, FMOD::Sound*> sounds;
    std::unordered_map<std::string, std::vector<FMOD::Channel*>> activeChannels;
    std::unordered_map<SoundInstanceID, FMOD::Channel*> instanceMap;

    std::queue<SoundInstanceID> reusableIDs;
    SoundInstanceID nextInstanceID;

    SoundInstanceID GenerateID();
};
