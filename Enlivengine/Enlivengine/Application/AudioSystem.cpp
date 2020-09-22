#include <Enlivengine/Application/AudioSystem.hpp>

namespace en
{

U32 Music::sMusicUIDGenerator = 0;
U32 Sound::sSoundUIDGenerator = 0;

en::SoundSourceStatus toEN(const sf::SoundSource::Status& status)
{
	switch (status)
	{
	case sf::SoundSource::Status::Playing: return SoundSourceStatus::Playing; break;
	case sf::SoundSource::Status::Paused: return SoundSourceStatus::Paused; break;
	case sf::SoundSource::Status::Stopped: return SoundSourceStatus::Stopped; break;
	}
	enAssert(false); // Unimplemented
	return SoundSourceStatus::Playing;
}

sf::SoundSource::Status toSF(const SoundSourceStatus& status)
{
	switch (status)
	{
	case SoundSourceStatus::Playing: return sf::SoundSource::Status::Playing; break;
	case SoundSourceStatus::Paused: return sf::SoundSource::Status::Paused; break;
	case SoundSourceStatus::Stopped: return sf::SoundSource::Status::Stopped; break;
	}
	// Unimplemented
	return sf::SoundSource::Status::Playing;
}

Music::Music(MusicID musicID, const std::string& filename, AudioSystem* audioSystem)
	: mMusic()
	, mAudioSystem(audioSystem)
	, mMusicID(InvalidMusicID)
	, mMusicUID(InvalidMusicUID)
	, mUserVolume(1.0f)
{
	if (musicID != InvalidMusicID && mMusic.openFromFile(filename))
	{
		mMusicID = musicID;
		mMusicUID = sMusicUIDGenerator++;
	}
}

bool Music::IsValid() const
{
	return mMusicID != InvalidMusicID && mMusicUID != InvalidMusicUID;
}

MusicID Music::GetMusicID() const
{
	return mMusicID;
}

U32 Music::GetUID() const
{
	return mMusicUID;
}

void Music::SetLoop(bool loop)
{
	mMusic.setLoop(loop);
}

bool Music::IsLoop() const
{
	return mMusic.getLoop();
}

void Music::SetVolume(F32 userVolume)
{
	mUserVolume = userVolume;
	if (mAudioSystem != nullptr)
	{
		userVolume *= mAudioSystem->GetCurrentMusicsVolume();
	}
	mMusic.setVolume(userVolume * 100.0f);
}

F32 Music::GetVolume() const
{
	return mUserVolume;
}

F32 Music::GetFinalVolume() const
{
	return 0.01f * mMusic.getVolume();
}

void Music::Play()
{
	mMusic.play();
}

void Music::Pause()
{
	mMusic.pause();
}

void Music::Stop()
{
	mMusic.stop();
	if (mAudioSystem != nullptr)
	{
		mAudioSystem->ForceStopMusic(mMusicUID);
	}
}

SoundSourceStatus Music::GetStatus() const
{
	return toEN(mMusic.getStatus());
}

void Music::UpdateManagerVolume(F32 managerVolume)
{
	mMusic.setVolume(managerVolume * mUserVolume * 100.0f);
}

MusicPtr::MusicPtr(AudioSystem* audioSystem, MusicID musicID, U32 musicUID)
	: mAudioSystem(audioSystem)
	, mMusicID(musicID)
	, mMusicUID(musicUID)
{
}

bool MusicPtr::IsValid() const
{
	return GetMusic() != nullptr;
}

MusicID MusicPtr::GetMusicID() const
{
	return mMusicID;
}

U32 MusicPtr::GetUID() const
{
	return mMusicUID;
}

void MusicPtr::SetLoop(bool loop)
{
	if (Music* music = GetMusic())
	{
		music->SetLoop(loop);
	}
}

bool MusicPtr::IsLoop() const
{
	if (const Music* music = GetMusic())
	{
		return music->IsLoop();
	}
	return false;
}

void MusicPtr::SetVolume(F32 userVolume)
{
	if (Music* music = GetMusic())
	{
		music->SetVolume(userVolume);
	}
}

F32 MusicPtr::GetVolume() const
{
	if (const Music* music = GetMusic())
	{
		return music->GetVolume();
	}
	return 0.0f;
}
F32 MusicPtr::GetFinalVolume() const
{
	if (const Music* music = GetMusic())
	{
		return music->GetFinalVolume();
	}
	return 0.0f;
}

void MusicPtr::Play()
{
	if (Music* music = GetMusic())
	{
		music->Play();
	}
}

void MusicPtr::Pause()
{
	if (Music* music = GetMusic())
	{
		music->Pause();
	}
}

void MusicPtr::Stop()
{
	if (Music* music = GetMusic())
	{
		music->Stop();
	}
}

SoundSourceStatus MusicPtr::GetStatus() const
{
	if (const Music* music = GetMusic())
	{
		return music->GetStatus();
	}
	return SoundSourceStatus::Stopped;
}

en::Music* MusicPtr::GetMusic()
{
	if (mAudioSystem != nullptr && mMusicID != InvalidMusicID && mMusicUID != InvalidMusicUID)
	{
		return mAudioSystem->GetMusicInternal(mMusicUID);
	}
	return nullptr;
}

const en::Music* MusicPtr::GetMusic() const
{
	if (mAudioSystem != nullptr && mMusicID != InvalidMusicID && mMusicUID != InvalidMusicUID)
	{
		return mAudioSystem->GetMusicInternal(mMusicUID);
	}
	return nullptr;
}

Sound::Sound(SoundBufferPtr soundBuffer, AudioSystem* audioSystem)
	: mSound()
	, mAudioSystem(audioSystem)
	, mSoundID(InvalidSoundID)
	, mSoundUID(InvalidSoundUID)
	, mUserVolume(1.0f)
{
	if (soundBuffer.IsValid())
	{
		mSound.setBuffer(soundBuffer.Get());
		mSoundID = static_cast<SoundID>(soundBuffer.GetID());
		mSoundUID = sSoundUIDGenerator++;
	}
}

bool Sound::IsValid() const
{
	return mSoundID != InvalidSoundID && mSoundUID != InvalidSoundUID;
}

SoundID Sound::GetSoundID() const
{
	return mSoundID;
}

U32 Sound::GetUID() const
{
	return mSoundUID;
}

void Sound::SetVolume(F32 userVolume)
{
	mUserVolume = userVolume;
	if (mAudioSystem != nullptr)
	{
		userVolume *= mAudioSystem->GetCurrentSoundsVolume();
	}
	mSound.setVolume(userVolume * 100.0f);
}

F32 Sound::GetVolume() const
{
	return mUserVolume;
}

F32 Sound::GetFinalVolume() const
{
	return 0.01f * mSound.getVolume();
}

void Sound::Play()
{
	mSound.play();
}

void Sound::Pause()
{
	mSound.pause();
}

void Sound::Stop()
{
	mSound.stop();
	if (mAudioSystem != nullptr)
	{
		mAudioSystem->ForceStopSound(mSoundUID);
	}
}

SoundSourceStatus Sound::GetStatus() const
{
	return toEN(mSound.getStatus());
}

void Sound::UpdateManagerVolume(F32 managerVolume)
{
	mSound.setVolume(managerVolume * mUserVolume * 100.0f);
}

SoundPtr::SoundPtr(AudioSystem* audioSystem, SoundID soundID, U32 soundUID)
	: mAudioSystem(audioSystem)
	, mSoundID(soundID)
	, mSoundUID(soundUID)
{
}

bool SoundPtr::IsValid() const
{
	return GetSound() != nullptr;
}

SoundID SoundPtr::GetSoundID() const
{
	return mSoundID;
}

U32 SoundPtr::GetUID() const
{
	return mSoundUID;
}

void SoundPtr::SetVolume(F32 userVolume)
{
	if (Sound* sound = GetSound())
	{
		sound->SetVolume(userVolume);
	}
}

F32 SoundPtr::GetVolume() const
{
	if (const Sound* sound = GetSound())
	{
		return sound->GetVolume();
	}
	return 0.0f;
}

F32 SoundPtr::GetFinalVolume() const
{
	if (const Sound* sound = GetSound())
	{
		return sound->GetFinalVolume();
	}
	return 0.0f;
}

void SoundPtr::Play()
{
	if (Sound* sound = GetSound())
	{
		sound->Play();
	}
}

void SoundPtr::Pause()
{
	if (Sound* sound = GetSound())
	{
		sound->Pause();
	}
}

void SoundPtr::Stop()
{
	if (Sound* sound = GetSound())
	{
		sound->Stop();
	}
}

SoundSourceStatus SoundPtr::GetStatus() const
{
	if (const Sound* sound = GetSound())
	{
		return sound->GetStatus();
	}
	return SoundSourceStatus::Stopped;
}

en::Sound* SoundPtr::GetSound()
{
	if (mAudioSystem != nullptr && mSoundID != InvalidSoundID && mSoundUID != InvalidSoundUID)
	{
		return mAudioSystem->GetSoundInternal(mSoundUID);
	}
	return nullptr;
}

const en::Sound* SoundPtr::GetSound() const
{
	if (mAudioSystem != nullptr && mSoundID != InvalidSoundID && mSoundUID != InvalidSoundUID)
	{
		return mAudioSystem->GetSoundInternal(mSoundUID);
	}
	return nullptr;
}

AudioSystem::AudioSystem()
	: mGlobalVolume(1.0f)
	, mGlobalEnabled(true)
	, mPlaying(true)
	, mMusicFilenames()
	, mMusicsVolume(1.0f)
	, mMusicsEnabled(true)
	, mMusics()
	, mLoadedSounds()
	, mSoundsVolume(1.0f)
	, mSoundsEnabled(true)
	, mSounds()
{
}

F32 AudioSystem::GetGlobalVolume() const
{
	return mGlobalVolume;
}

void AudioSystem::SetGlobalVolume(F32 volume)
{
	mGlobalVolume = volume;
	sf::Listener::setGlobalVolume(volume);
}

bool AudioSystem::IsEnabled() const
{
	return mGlobalEnabled;
}

void AudioSystem::SetEnabled(bool enabled)
{
	mGlobalEnabled = enabled;
	UpdateMusicsVolume();
	UpdateSoundsVolume();
}

void AudioSystem::Play()
{
	mPlaying = true;
	PlayMusics();
	PlaySounds();
}

void AudioSystem::Pause()
{
	mPlaying = false;
	PauseMusics();
	PauseSounds();
}

void AudioSystem::Stop()
{
	StopMusics();
	StopSounds();
}

bool AudioSystem::IsPlaying() const
{
	return mPlaying;
}

bool AudioSystem::IsInactive() const
{
	return (GetCurrentMusicsCount() + GetCurrentSoundsCount()) == 0;
}

void AudioSystem::Clear()
{
	ReleaseSounds();
}

F32 AudioSystem::GetMusicVolume() const
{
	return mMusicsVolume;
}

void AudioSystem::SetMusicVolume(F32 volume)
{
	mMusicsVolume = volume;
	UpdateMusicsVolume();
}

bool AudioSystem::AreMusicsEnabled() const
{
	return mMusicsEnabled;
}

void AudioSystem::SetMusicsEnabled(bool enabled)
{
	mMusicsEnabled = enabled;
	UpdateMusicsVolume();
}

F32 AudioSystem::GetCurrentMusicsVolume() const
{
	if (!mGlobalEnabled || !mMusicsEnabled)
	{
		return 0.0f;
	}
	else
	{
		return mMusicsVolume;
	}
}

MusicID AudioSystem::PrepareMusic(const char* id, const std::string& filename)
{
	MusicID index(ResourceManager::StringToResourceID(id));
	if (mMusicFilenames.find(index) == mMusicFilenames.end())
	{
		mMusicFilenames[index] = filename;
	}
	return index;
}

MusicPtr AudioSystem::PlayMusic(MusicID id, bool loop /*= true*/)
{
	const auto itr = mMusicFilenames.find(id);
	if (mMusics.size() < MAX_MUSICS && itr != mMusicFilenames.end())
	{
		mMusics.push_back(new Music(id, itr->second, this));
		Music* music = mMusics.back();
		if (music != nullptr && music->IsValid())
		{
			music->SetLoop(loop);
			music->UpdateManagerVolume(GetCurrentMusicsVolume());
			music->Play();
			if (!mPlaying)
			{
				music->Pause();
			}
			return MusicPtr(this, music->GetMusicID(), music->GetUID());
		}
	}
	return MusicPtr();
}

MusicPtr AudioSystem::PlayMusic(const char* id, bool loop /*= true*/)
{
	return PlayMusic(ResourceManager::StringToResourceID(id), loop);
}

U32 AudioSystem::GetCurrentMusicsCount() const
{
	return static_cast<U32>(mMusics.size());
}

void AudioSystem::PlayMusics()
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMusics[i]->Play();
	}
}

void AudioSystem::PauseMusics()
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMusics[i]->Pause();
	}
}

void AudioSystem::StopMusics()
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		delete mMusics[i];
		mMusics[i] = nullptr;
	}
	mMusics.clear();
}

F32 AudioSystem::GetSoundVolume() const
{
	return mSoundsVolume;
}

void AudioSystem::SetSoundVolume(F32 volume)
{
	mSoundsVolume = volume;
	UpdateSoundsVolume();
}

bool AudioSystem::AreSoundsEnabled() const
{
	return mSoundsEnabled;
}

void AudioSystem::SetSoundsEnabled(bool enabled)
{
	mSoundsEnabled = enabled;
	UpdateSoundsVolume();
}

F32 AudioSystem::GetCurrentSoundsVolume() const
{
	if (!mGlobalEnabled || !mSoundsEnabled)
	{
		return 0.0f;
	}
	else
	{
		return mSoundsVolume;
	}
}

SoundID AudioSystem::PrepareSound(const char* id, const std::string& filename)
{
	const SoundBufferPtr soundBuffer = ResourceManager::GetInstance().Create(id, SoundBufferLoader::FromFile(filename));
	if (soundBuffer.IsValid())
	{
		const SoundID soundId = soundBuffer.GetID();
		if (!IsSoundLoaded(soundId))
		{
			mLoadedSounds.push_back(soundId);
		}
		return soundId;
	}
	else
	{
		return InvalidResourceID;
	}
}

bool AudioSystem::IsSoundLoaded(SoundID id) const
{
	const size_t size = mLoadedSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (mLoadedSounds[i] == id)
		{
			return true;
		}
	}
	return false;
}

bool AudioSystem::IsSoundLoaded(const char* id) const
{
	return IsSoundLoaded(ResourceManager::StringToResourceID(id));
}

U32 AudioSystem::GetLoadedSoundsCount() const
{
	return static_cast<U32>(mLoadedSounds.size());
}

SoundPtr AudioSystem::PlaySound(SoundID id)
{
	if (mSounds.size() < MAX_SOUNDS)
	{
		const SoundBufferPtr soundBuffer = ResourceManager::GetInstance().Get<SoundBuffer>(id);
		if (soundBuffer.IsValid())
		{
			mSounds.push_back(new Sound(soundBuffer, this));
			Sound* sound = mSounds.back();
			if (sound != nullptr && sound->IsValid())
			{
				sound->UpdateManagerVolume(GetCurrentSoundsVolume());
				sound->Play();
				if (!mPlaying)
				{
					sound->Pause();
				}
				return SoundPtr(this, sound->GetSoundID(), sound->GetUID());
			}
		}
	}
	return SoundPtr();
}

SoundPtr AudioSystem::PlaySound(const char* id)
{
	return PlaySound(ResourceManager::StringToResourceID(id));
}

U32 AudioSystem::GetCurrentSoundsCount() const
{
	return static_cast<U32>(mLoadedSounds.size());
}

void AudioSystem::ReleaseSound(SoundID id)
{
	const size_t size = mLoadedSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (mLoadedSounds[i] == id)
		{
			ResourceManager::GetInstance().Release<SoundBuffer>(id);
			mLoadedSounds.erase(mLoadedSounds.begin() + i);
			return;
		}
	}
}

void AudioSystem::ReleaseSound(const char* id)
{
	ReleaseSound(ResourceManager::StringToResourceID(id));
}

void AudioSystem::PlaySounds()
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		mSounds[i]->Play();
	}
}

void AudioSystem::PauseSounds()
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		mSounds[i]->Pause();
	}
}

void AudioSystem::StopSounds()
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		delete mSounds[i];
		mSounds[i] = nullptr;
	}
	mSounds.clear();
}

void AudioSystem::ReleaseSounds()
{
	const size_t size = mLoadedSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		ResourceManager::GetInstance().Release<SoundBuffer>(mLoadedSounds[i]);
	}
	mLoadedSounds.clear();
}

void AudioSystem::Update()
{
	if (!mPlaying)
		return;

	size_t musicsSize = mMusics.size();
	for (size_t i = 0; i < musicsSize; )
	{
		if (mMusics[i]->GetStatus() == SoundSourceStatus::Stopped)
		{
			delete mMusics[i];
			mMusics[i] = nullptr;
			mMusics.erase(mMusics.begin() + i);
			musicsSize--;
		}
		else
		{
			i++;
		}
	}

	size_t soundsSize = mSounds.size();
	for (size_t i = 0; i < soundsSize; )
	{
		if (mSounds[i]->GetStatus() == SoundSourceStatus::Stopped)
		{
			delete mSounds[i];
			mSounds[i] = nullptr;
			mSounds.erase(mSounds.begin() + i);
			soundsSize--;
		}
		else
		{
			i++;
		}
	}
}

void AudioSystem::ForceStopMusic(U32 musicUID)
{
	for (size_t i = 0; i < mMusics.size(); ++i)
	{
		if (mMusics[i]->GetUID() == musicUID)
		{
			delete mMusics[i];
			mMusics[i] = nullptr;
			mMusics.erase(mMusics.begin() + i);
			return;
		}
	}
}

en::Music* AudioSystem::GetMusicInternal(U32 musicUID)
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (mMusics[i]->GetUID() == musicUID)
		{
			return mMusics[i];
		}
	}
	return nullptr;
}

const en::Music* AudioSystem::GetMusicInternal(U32 musicUID) const
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (mMusics[i]->GetUID() == musicUID)
		{
			return mMusics[i];
		}
	}
	return nullptr;
}

void AudioSystem::ForceStopSound(U32 soundUID)
{
	for (size_t i = 0; i < mSounds.size(); ++i)
	{
		if (mSounds[i]->GetUID() == soundUID)
		{
			delete mSounds[i];
			mSounds[i] = nullptr;
			mSounds.erase(mSounds.begin() + i);
			return;
		}
	}
}

en::Sound* AudioSystem::GetSoundInternal(U32 soundUID)
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (mSounds[i]->GetUID() == soundUID)
		{
			return mSounds[i];
		}
	}
	return nullptr;
}

const en::Sound* AudioSystem::GetSoundInternal(U32 soundUID) const
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (mSounds[i]->GetUID() == soundUID)
		{
			return mSounds[i];
		}
	}
	return nullptr;
}

void AudioSystem::UpdateMusicsVolume()
{
	const F32 currentVolume = GetCurrentMusicsVolume();
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMusics[i]->UpdateManagerVolume(currentVolume);
	}
}

void AudioSystem::UpdateSoundsVolume()
{
	const F32 currentVolume = GetCurrentSoundsVolume();
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		mSounds[i]->UpdateManagerVolume(currentVolume);
	}
}

} // namespace en
