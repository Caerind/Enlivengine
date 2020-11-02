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

using MusicID = U32;
constexpr MusicID InvalidMusicID = U32_Max;
constexpr U32 InvalidMusicUID = U32_Max;

// It's recommended to not use Music directly
// Use the MusicManager/AudioManager + MusicPtr
class Music
{
public:
	// TODO : AUDIO UPDATE
	//Music(MusicID musicID, const std::string& filename);

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
	AudioStatus GetStatus() const;

private:
	friend class MusicManager;
	void UpdateManagerVolume(F32 managerVolume);

private:
	// TODO : AUDIO UPDATE
	//sf::Music mMusic;

	AudioStatus mStatus;
	MusicID mMusicID;
	U32 mMusicUID;
	F32 mUserVolume;

	static U32 sMusicUIDGenerator;
};

// This class is a Ptr to a music managed by the MusicManager
class MusicPtr
{
public:
	MusicPtr(MusicID musicID = InvalidMusicID, U32 musicUID = InvalidMusicUID);

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
	AudioStatus GetStatus() const;

private:
	Music* GetMusic();
	const Music* GetMusic() const;

private:
	MusicID mMusicID;
	U32 mMusicUID;
};

} // namespace en

#endif // ENLIVE_MODULE_AUDIO