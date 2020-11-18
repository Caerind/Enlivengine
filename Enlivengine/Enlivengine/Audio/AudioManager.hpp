#pragma once

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Audio/AudioStatus.hpp>
#include <Enlivengine/Audio/MusicManager.hpp>
#include <Enlivengine/Audio/SoundManager.hpp>

// TODO : Add more functions to manipulate Music/Sound by ID/UID

namespace en
{

// Class that smartly manage audio sources
class AudioManager
{
	ENLIVE_SINGLETON(AudioManager);

public:
	// AudioManager
	F32 GetGlobalVolume() const;
	void SetGlobalVolume(F32 volume);
	bool IsEnabled() const;
	void SetEnabled(bool enabled);
	void Play();
	void Pause();
	void Stop(); // Stop everything, but keep the same play/pause state
	bool IsPlaying() const;
	bool IsInactive() const; // No sounds and no musics
	void Clear();
	void Update();

	// MusicManager
	F32 GetMusicVolume() const;
	void SetMusicVolume(F32 volume);
	bool AreMusicsEnabled() const;
	void SetMusicsEnabled(bool enabled);
	F32 GetCurrentMusicsVolume() const;
	MusicID PrepareMusic(const char* id, const std::string& filename);
	MusicPtr PlayMusic(MusicID id, bool loop = true);
	MusicPtr PlayMusic(const char* id, bool loop = true);
	U32 GetCurrentMusicsCount() const;
	void PlayMusics();
	void PauseMusics();
	void StopMusics();

	// SoundManager
	F32 GetSoundVolume() const;
	void SetSoundVolume(F32 volume);
	bool AreSoundsEnabled() const;
	void SetSoundsEnabled(bool enabled);
	F32 GetCurrentSoundsVolume() const;
	SoundID PrepareSound(const char* id, const std::string& filename);
	bool IsSoundLoaded(SoundID id) const;
	bool IsSoundLoaded(const char* id) const;
	U32 GetLoadedSoundsCount() const;
	SoundPtr PlaySound(SoundID id);
	SoundPtr PlaySound(const char* id);
	U32 GetCurrentSoundsCount() const;
	void ReleaseSound(SoundID id);
	void ReleaseSound(const char* id);
	void PlaySounds();
	void PauseSounds();
	void StopSounds();
	void ReleaseSounds();

private:
	F32 mGlobalVolume;
	bool mGlobalEnabled;
	bool mPlaying;
};

} // namespace en