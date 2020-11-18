#pragma once

#include <vector>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Audio/AudioStatus.hpp>
#include <Enlivengine/Audio/Sound.hpp>

namespace en
{

class SoundManager
{
	ENLIVE_SINGLETON(SoundManager);

public:
	static constexpr U32 kMaxSounds = 240;

	F32 GetSoundVolume() const;
	void SetSoundVolume(F32 volume);
	bool AreSoundsEnabled() const;
	void SetSoundsEnabled(bool enabled);

	SoundID PrepareSound(const char* id, const std::string& filename);
	bool IsSoundLoaded(SoundID id) const;
	bool IsSoundLoaded(const char* id) const;
	void ReleaseSound(SoundID id);
	void ReleaseSound(const char* id);
	U32 GetLoadedSoundsCount() const;

	SoundPtr PlaySound(SoundID id);
	SoundPtr PlaySound(const char* id);

	void Play();
	void Pause();
	void Stop();
	void Release();

	F32 GetCurrentSoundsVolume() const;
	U32 GetCurrentSoundsCount() const;

private:
	friend class AudioManager;
	void Update();

	friend class Sound;
	void ForceStopSound(U32 soundUID);

	friend class SoundPtr;
	Sound* GetSoundInternal(U32 soundUID);
	const Sound* GetSoundInternal(U32 soundUID) const;

	void UpdateSoundsVolume();

private:
	F32 mSoundsVolume;
	bool mSoundsEnabled;

	std::vector<SoundID> mLoadedSounds;
	std::vector<Sound*> mSounds;
};

} // namespace en