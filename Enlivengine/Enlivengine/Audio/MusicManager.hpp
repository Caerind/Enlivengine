#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_AUDIO

#include <unordered_map>
#include <vector>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Audio/AudioStatus.hpp>
#include <Enlivengine/Audio/Music.hpp>

namespace en
{

class MusicManager
{
	ENLIVE_SINGLETON(MusicManager);

public:
	static constexpr U32 kMaxMusics = 16;

	F32 GetMusicVolume() const;
	void SetMusicVolume(F32 volume);
	bool AreMusicsEnabled() const;
	void SetMusicsEnabled(bool enabled);

	MusicID PrepareMusic(const char* id, const std::string& filename);

	MusicPtr PlayMusic(MusicID id, bool loop = true);
	MusicPtr PlayMusic(const char* id, bool loop = true);

	F32 GetCurrentMusicsVolume() const;
	U32 GetCurrentMusicsCount() const;

	void Play();
	void Pause();
	void Stop();

private:
	friend class AudioManager;
	void Update();

	friend class Music;
	void ForceStopMusic(U32 musicUID);

	friend class MusicPtr;
	Music* GetMusicInternal(U32 musicUID);
	const Music* GetMusicInternal(U32 musicUID) const;

	void UpdateMusicsVolume();

private:
	F32 mMusicsVolume;
	bool mMusicsEnabled;

	std::unordered_map<MusicID, std::string> mMusicFilenames;
	std::vector<Music*> mMusics;
};

} // namespace en

#endif // ENLIVE_MODULE_AUDIO