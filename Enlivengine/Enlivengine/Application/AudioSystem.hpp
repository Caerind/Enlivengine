#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <memory>
#include <vector>

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <Enlivengine/Graphics/SFMLResources.hpp>

// TODO : Add more functions to AudioSystem to manipulate Music/Sound by ID/UID
// TODO : Split into MusicManager & SoundManager ?

namespace en
{

class AudioSystem;

enum class SoundSourceStatus
{
	Playing,
	Paused,
	Stopped
};
SoundSourceStatus toEN(const sf::SoundSource::Status& status);
sf::SoundSource::Status toSF(const SoundSourceStatus& status);

using MusicID = U32;
constexpr MusicID InvalidMusicID = U32_Max;
constexpr U32 InvalidMusicUID = U32_Max;

// It's recommend to not use Music directly
// Use the AudioSystem + MusicPtr
class Music
{
public:
	Music(MusicID musicID, const std::string& filename, AudioSystem* audioSystem = nullptr);

	bool IsValid() const;
	MusicID GetMusicID() const;
	U32 GetUID() const;

	void SetLoop(bool loop);
	bool IsLoop() const;

	void SetVolume(F32 userVolume);
	F32 GetVolume() const;
	F32 GetFinalVolume() const;

	void Play();
	void Pause();
	void Stop();
	SoundSourceStatus GetStatus() const;

private:
	friend class AudioSystem;
	void UpdateManagerVolume(F32 managerVolume);

private:
	sf::Music mMusic;
	AudioSystem* mAudioSystem;

	MusicID mMusicID;
	U32 mMusicUID;

	F32 mUserVolume;

	static U32 sMusicUIDGenerator;
};

// This class is a Ptr to a music managed by the AudioSystem
class MusicPtr
{
public:
	MusicPtr(AudioSystem* audioSystem = nullptr, MusicID musicID = InvalidMusicID, U32 musicUID = InvalidMusicUID);

	bool IsValid() const;
	MusicID GetMusicID() const;
	U32 GetUID() const;

	void SetLoop(bool loop);
	bool IsLoop() const;

	void SetVolume(F32 userVolume);
	F32 GetVolume() const;
	F32 GetFinalVolume() const;

	void Play();
	void Pause();
	void Stop();
	SoundSourceStatus GetStatus() const;

private:
	Music* GetMusic();
	const Music* GetMusic() const;

private:
	AudioSystem* mAudioSystem;
	MusicID mMusicID;
	U32 mMusicUID;
};

using SoundID = U32;
constexpr SoundID InvalidSoundID = U32_Max;
constexpr U32 InvalidSoundUID = U32_Max;

// It's recommend to not use Sound directly
// Use the AudioSystem + SoundPtr
class Sound
{
public:
	Sound(SoundBufferPtr soundBuffer, AudioSystem* audioSystem = nullptr);

	bool IsValid() const;
	SoundID GetSoundID() const;
	U32 GetUID() const;

	void SetVolume(F32 userVolume);
	F32 GetVolume() const;
	F32 GetFinalVolume() const;

	void Play();
	void Pause();
	void Stop();
	SoundSourceStatus GetStatus() const;

private:
	friend class AudioSystem;
	void UpdateManagerVolume(F32 managerVolume);

private:
	sf::Sound mSound;
	AudioSystem* mAudioSystem;

	SoundID mSoundID;
	U32 mSoundUID;

	F32 mUserVolume;

	static U32 sSoundUIDGenerator;
};

// This class is a Ptr to a sound managed by the AudioSystem
class SoundPtr
{
public:
	SoundPtr(AudioSystem* audioSystem = nullptr, SoundID soundID = InvalidSoundID, U32 soundUID = InvalidSoundUID);

	bool IsValid() const;
	SoundID GetSoundID() const;
	U32 GetUID() const;

	void SetVolume(F32 userVolume);
	F32 GetVolume() const;
	F32 GetFinalVolume() const;

	void Play();
	void Pause();
	void Stop();
	SoundSourceStatus GetStatus() const;

private:
	Sound* GetSound();
	const Sound* GetSound() const;

private:
	AudioSystem* mAudioSystem;
	SoundID mSoundID;
	U32 mSoundUID;
};

// Class that smartly manage audio sources
class AudioSystem
{
	ENLIVE_SINGLETON(AudioSystem);

public:
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
	friend class Application;
	void Update();

	friend class Music;
	void ForceStopMusic(U32 musicUID);

	friend class MusicPtr;
	en::Music* GetMusicInternal(U32 musicUID);
	const en::Music* GetMusicInternal(U32 musicUID) const;

	friend class Sound;
	void ForceStopSound(U32 soundUID);

	friend class SoundPtr;
	en::Sound* GetSoundInternal(U32 soundUID);
	const en::Sound* GetSoundInternal(U32 soundUID) const;

	void UpdateMusicsVolume();
	void UpdateSoundsVolume();

private:
	F32 mGlobalVolume;
	bool mGlobalEnabled;
	bool mPlaying;

	std::unordered_map<MusicID, std::string> mMusicFilenames;
	F32 mMusicsVolume;
	bool mMusicsEnabled;
	std::vector<Music*> mMusics;

	std::vector<SoundID> mLoadedSounds;
	F32 mSoundsVolume;
	bool mSoundsEnabled;
	std::vector<Sound*> mSounds;

	static constexpr U32 MAX_MUSICS = 16;
	static constexpr U32 MAX_SOUNDS = 240;
};

} // namespace en
