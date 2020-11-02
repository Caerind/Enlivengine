#include <Enlivengine/Audio/SoundManager.hpp>

#ifdef ENLIVE_MODULE_AUDIO

namespace en
{

SoundManager::SoundManager()
	: mSoundsVolume(1.0f)
	, mSoundsEnabled(true)
	, mLoadedSounds()
	, mSounds()
{
}

F32 SoundManager::GetSoundVolume() const
{
	return mSoundsVolume;
}

void SoundManager::SetSoundVolume(F32 volume)
{
	mSoundsVolume = volume;
	UpdateSoundsVolume();
}

bool SoundManager::AreSoundsEnabled() const
{
	return mSoundsEnabled;
}

void SoundManager::SetSoundsEnabled(bool enabled)
{
	mSoundsEnabled = enabled;
	UpdateSoundsVolume();
}

SoundID SoundManager::PrepareSound(const char* id, const std::string& filename)
{
	ENLIVE_UNUSED(id);
	ENLIVE_UNUSED(filename);
	/*
	// TODO : AUDIO UPDATE
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
		return InvalidSoundID;
	}
	*/
	return InvalidSoundID;
}

bool SoundManager::IsSoundLoaded(SoundID id) const
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

bool SoundManager::IsSoundLoaded(const char* id) const
{
	ENLIVE_UNUSED(id);
	/*
	// TODO : AUDIO UPDATE
	return IsSoundLoaded(ResourceManager::StringToResourceID(id));
	*/
	return false;
}

void SoundManager::ReleaseSound(SoundID id)
{
	ENLIVE_UNUSED(id);
	/*
	// TODO : AUDIO UPDATE
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
	*/
}

void SoundManager::ReleaseSound(const char* id)
{
	ENLIVE_UNUSED(id);
	/*
	// TODO : AUDIO UPDATE
	ReleaseSound(ResourceManager::StringToResourceID(id));
	*/
}

U32 SoundManager::GetLoadedSoundsCount() const
{
	return static_cast<U32>(mLoadedSounds.size());
}

SoundPtr SoundManager::PlaySound(SoundID id)
{
	ENLIVE_UNUSED(id);
	/*
	// TODO : AUDIO UPDATE
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
	*/
	return SoundPtr();
}

SoundPtr SoundManager::PlaySound(const char* id)
{
	ENLIVE_UNUSED(id);
	/*
	// TODO : AUDIO UPDATE
	return PlaySound(ResourceManager::StringToResourceID(id));
	*/
	return SoundPtr();
}

void SoundManager::Play()
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		mSounds[i]->Play();
	}
}

void SoundManager::Pause()
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		mSounds[i]->Pause();
	}
}

void SoundManager::Stop()
{
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		delete mSounds[i];
		mSounds[i] = nullptr;
	}
	mSounds.clear();
}

void SoundManager::Release()
{
	/*
	// TODO : AUDIO UPDATE
	const size_t size = mLoadedSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		ResourceManager::GetInstance().Release<SoundBuffer>(mLoadedSounds[i]);
	}
	mLoadedSounds.clear();
	*/
}

F32 SoundManager::GetCurrentSoundsVolume() const
{
	if (!mSoundsEnabled)
	{
		return 0.0f;
	}
	else
	{
		return mSoundsVolume;
	}
}

U32 SoundManager::GetCurrentSoundsCount() const
{
	return static_cast<U32>(mLoadedSounds.size());
}

void SoundManager::Update()
{
	size_t soundsSize = mSounds.size();
	for (size_t i = 0; i < soundsSize; )
	{
		if (mSounds[i]->GetStatus() == AudioStatus::Stopped)
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

void SoundManager::ForceStopSound(U32 soundUID)
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

Sound* SoundManager::GetSoundInternal(U32 soundUID)
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

const Sound* SoundManager::GetSoundInternal(U32 soundUID) const
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

void SoundManager::UpdateSoundsVolume()
{
	const F32 currentVolume = GetCurrentSoundsVolume();
	const size_t size = mSounds.size();
	for (size_t i = 0; i < size; ++i)
	{
		mSounds[i]->UpdateManagerVolume(currentVolume);
	}
}

} // namespace en

#endif // ENLIVE_MODULE_AUDIO