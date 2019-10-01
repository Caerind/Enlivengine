#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <memory>
#include <vector>

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/Graphics/SFMLResources.hpp>

namespace en
{

using MusicId = U32;
using SoundId = U32;

class AudioSystem
{
    public:
        using MusicPtr = std::shared_ptr<sf::Music>;
        using SoundPtr = std::shared_ptr<sf::Sound>;

    public:
        AudioSystem();

		MusicId createMusic(const char* id, const std::string& filename);
		MusicPtr playMusic(MusicId id, bool loop = true);
		MusicPtr playMusic(const char* id, bool loop = true);

		SoundId createSound(const char* id, const std::string& filename);
        SoundPtr playSound(SoundId id);
		SoundPtr playSound(const char* id);

        void play();
        void pause();
        void stop();

        void update();

        void setGlobalVolume(F32 volume);
        void setMusicVolume(F32 volume);
        void setSoundVolume(F32 volume);
        F32 getGlobalVolume() const;
        F32 getMusicVolume() const;
        F32 getSoundVolume() const;

        sf::SoundSource::Status getStatus() const;

    private:
		std::unordered_map<MusicId, std::string> mMusicFilenames;
		SoundManager mSoundBuffers;

		std::vector<MusicPtr> mMusics;
		std::vector<SoundPtr> mSounds;
        sf::SoundSource::Status mStatus;

        F32 mMusicVolume;
        F32 mSoundVolume;

		static const U32 MAX_MUSIC = 16;
		static const U32 MAX_SOUND = 240;
};

} // namespace en
