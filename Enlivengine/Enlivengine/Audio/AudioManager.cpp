#include <Enlivengine/Audio/AudioManager.hpp>

#include <Enlivengine/Audio/MusicManager.hpp>
#include <Enlivengine/Audio/SoundManager.hpp>

namespace en
{

AudioManager::AudioManager()
	: mGlobalVolume(1.0f)
	, mGlobalEnabled(true)
	, mPlaying(true)
{
}

F32 AudioManager::GetGlobalVolume() const
{
	return mGlobalVolume;
}

void AudioManager::SetGlobalVolume(F32 volume)
{
	mGlobalVolume = volume;
	if (mGlobalEnabled)
	{
		//sf::Listener::setGlobalVolume(mGlobalVolume);
	}
	else
	{
		//sf::Listener::setGlobalVolume(0.0f);
	}
}

bool AudioManager::IsEnabled() const
{
	return mGlobalEnabled;
}

void AudioManager::SetEnabled(bool enabled)
{
	mGlobalEnabled = enabled;
	if (mGlobalEnabled)
	{
		//sf::Listener::setGlobalVolume(mGlobalVolume);
	}
	else
	{
		//sf::Listener::setGlobalVolume(0.0f);
	}
}

void AudioManager::Play()
{
	mPlaying = true;
	PlayMusics();
	PlaySounds();
}

void AudioManager::Pause()
{
	mPlaying = false;
	PauseMusics();
	PauseSounds();
}

void AudioManager::Stop()
{
	StopMusics();
	StopSounds();
}

bool AudioManager::IsPlaying() const
{
	return mPlaying;
}

bool AudioManager::IsInactive() const
{
	return (GetCurrentMusicsCount() + GetCurrentSoundsCount()) == 0;
}

void AudioManager::Clear()
{
	ReleaseSounds();
}

void AudioManager::Update()
{
	if (!mPlaying)
		return;

	MusicManager::GetInstance().Update();
	SoundManager::GetInstance().Update();
}

F32 AudioManager::GetMusicVolume() const
{
	return MusicManager::GetInstance().GetMusicVolume();
}

void AudioManager::SetMusicVolume(F32 volume)
{
	MusicManager::GetInstance().SetMusicVolume(volume);
}

bool AudioManager::AreMusicsEnabled() const
{
	return MusicManager::GetInstance().AreMusicsEnabled();
}

void AudioManager::SetMusicsEnabled(bool enabled)
{
	MusicManager::GetInstance().SetMusicsEnabled(enabled);
}

F32 AudioManager::GetCurrentMusicsVolume() const
{
	return MusicManager::GetInstance().GetCurrentMusicsVolume();
}

MusicID AudioManager::PrepareMusic(const char* id, const std::string& filename)
{
	return MusicManager::GetInstance().PrepareMusic(id, filename);
}

MusicPtr AudioManager::PlayMusic(MusicID id, bool loop /*= true*/)
{
	return MusicManager::GetInstance().PlayMusic(id, loop);
}

MusicPtr AudioManager::PlayMusic(const char* id, bool loop /*= true*/)
{
	ENLIVE_UNUSED(id);
	ENLIVE_UNUSED(loop);
	/*
	// TODO : AUDIO UPDATE
	return PlayMusic(ResourceManager::StringToResourceID(id), loop);
	*/
	return MusicPtr();
}

U32 AudioManager::GetCurrentMusicsCount() const
{
	return MusicManager::GetInstance().GetCurrentMusicsCount();
}

void AudioManager::PlayMusics()
{
	MusicManager::GetInstance().Play();
}

void AudioManager::PauseMusics()
{
	MusicManager::GetInstance().Pause();
}

void AudioManager::StopMusics()
{
	MusicManager::GetInstance().Stop();
}

F32 AudioManager::GetSoundVolume() const
{
	return SoundManager::GetInstance().GetSoundVolume();
}

void AudioManager::SetSoundVolume(F32 volume)
{
	SoundManager::GetInstance().SetSoundVolume(volume);
}

bool AudioManager::AreSoundsEnabled() const
{
	return SoundManager::GetInstance().AreSoundsEnabled();;
}

void AudioManager::SetSoundsEnabled(bool enabled)
{
	SoundManager::GetInstance().SetSoundsEnabled(enabled);
}

F32 AudioManager::GetCurrentSoundsVolume() const
{
	return SoundManager::GetInstance().GetCurrentSoundsVolume();
}

SoundID AudioManager::PrepareSound(const char* id, const std::string& filename)
{
	return SoundManager::GetInstance().PrepareSound(id, filename);
}

bool AudioManager::IsSoundLoaded(SoundID id) const
{
	return SoundManager::GetInstance().IsSoundLoaded(id);
}

bool AudioManager::IsSoundLoaded(const char* id) const
{
	return SoundManager::GetInstance().IsSoundLoaded(id);
}

U32 AudioManager::GetLoadedSoundsCount() const
{
	return SoundManager::GetInstance().GetLoadedSoundsCount();
}

SoundPtr AudioManager::PlaySound(SoundID id)
{
	return SoundManager::GetInstance().PlaySound(id);
}

SoundPtr AudioManager::PlaySound(const char* id)
{
	return SoundManager::GetInstance().PlaySound(id);
}

U32 AudioManager::GetCurrentSoundsCount() const
{
	return SoundManager::GetInstance().GetCurrentSoundsCount();
}

void AudioManager::ReleaseSound(SoundID id)
{
	SoundManager::GetInstance().ReleaseSound(id);
}

void AudioManager::ReleaseSound(const char* id)
{
	SoundManager::GetInstance().ReleaseSound(id);
}

void AudioManager::PlaySounds()
{
	SoundManager::GetInstance().Play();
}

void AudioManager::PauseSounds()
{
	SoundManager::GetInstance().Pause();
}

void AudioManager::StopSounds()
{
	SoundManager::GetInstance().Stop();
}

void AudioManager::ReleaseSounds()
{
	SoundManager::GetInstance().Release();
}

} // namespace en