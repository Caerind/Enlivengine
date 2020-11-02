#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_AUDIO

#include <unordered_map>
#include <memory>
#include <vector>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Audio/AudioStatus.hpp>

namespace en
{

using SoundID = U32;
constexpr SoundID InvalidSoundID = U32_Max;
constexpr U32 InvalidSoundUID = U32_Max;

// It's recommended to not use Sound directly
// Use the SoundManager/AudioManager + SoundPtr
class Sound
{
public:
	// TODO : AUDIO UPDATE
	//Sound(SoundBufferPtr soundBuffer);

	bool IsValid() const;
	SoundID GetSoundID() const;
	U32 GetUID() const;

	void SetVolume(F32 userVolume);
	F32 GetVolume() const;
	F32 GetFinalVolume() const;

	void Play();
	void Pause();
	void Stop();
	AudioStatus GetStatus() const;

private:
	friend class SoundManager;
	void UpdateManagerVolume(F32 managerVolume);

private:
	// TODO : AUDIO UPDATE
	//sf::Sound mSound;

	AudioStatus mStatus;
	SoundID mSoundID;
	U32 mSoundUID;
	F32 mUserVolume;

	static U32 sSoundUIDGenerator;
};

// This class is a Ptr to a sound managed by the SoundManager
class SoundPtr
{
public:
	SoundPtr(SoundID soundID = InvalidSoundID, U32 soundUID = InvalidSoundUID);

	bool IsValid() const;
	SoundID GetSoundID() const;
	U32 GetUID() const;

	void SetVolume(F32 userVolume);
	F32 GetVolume() const;
	F32 GetFinalVolume() const;

	void Play();
	void Pause();
	void Stop();
	AudioStatus GetStatus() const;

private:
	Sound* GetSound();
	const Sound* GetSound() const;

private:
	SoundID mSoundID;
	U32 mSoundUID;
};

} // namespace en

#endif // ENLIVE_MODULE_AUDIO