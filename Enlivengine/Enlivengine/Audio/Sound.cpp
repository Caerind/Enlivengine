#include <Enlivengine/Audio/Sound.hpp>

#include <Enlivengine/Audio/SoundManager.hpp>

namespace en
{

U32 Sound::sSoundUIDGenerator = 0;

/*
// TODO : AUDIO UPDATE
Sound::Sound(SoundBufferPtr soundBuffer)
	: mSound()
	, mStatus(AudioStatus::Stopped)
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
*/

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
	// TODO : AUDIO UPDATE
	//mSound.setVolume(userVolume * AudioSystem::GetInstance().GetCurrentSoundsVolume() * 100.0f);
}

F32 Sound::GetVolume() const
{
	return mUserVolume;
}

F32 Sound::GetFinalVolume() const
{
	// TODO : AUDIO UPDATE
	//return 0.01f * mSound.getVolume();
	return 1.0f;
}

void Sound::Play()
{
	// TODO : AUDIO UPDATE
	//mSound.play();
	mStatus = AudioStatus::Playing;
}

void Sound::Pause()
{
	// TODO : AUDIO UPDATE
	//mSound.pause();
	mStatus = AudioStatus::Paused;
}

void Sound::Stop()
{
	// TODO : AUDIO UPDATE
	//mSound.stop();
	mStatus = AudioStatus::Stopped;
	SoundManager::GetInstance().ForceStopSound(mSoundUID);
}

AudioStatus Sound::GetStatus() const
{
	return mStatus;
}

void Sound::UpdateManagerVolume(F32 managerVolume)
{
	ENLIVE_UNUSED(managerVolume);
	// TODO : AUDIO UPDATE
	//mSound.setVolume(managerVolume * mUserVolume * 100.0f);
}

SoundPtr::SoundPtr(SoundID soundID, U32 soundUID)
	: mSoundID(soundID)
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

AudioStatus SoundPtr::GetStatus() const
{
	if (const Sound* sound = GetSound())
	{
		return sound->GetStatus();
	}
	return AudioStatus::Stopped;
}

Sound* SoundPtr::GetSound()
{
	if (mSoundID != InvalidSoundID && mSoundUID != InvalidSoundUID)
	{
		return SoundManager::GetInstance().GetSoundInternal(mSoundUID);
	}
	return nullptr;
}

const Sound* SoundPtr::GetSound() const
{
	if (mSoundID != InvalidSoundID && mSoundUID != InvalidSoundUID)
	{
		return SoundManager::GetInstance().GetSoundInternal(mSoundUID);
	}
	return nullptr;
}

} // namespace en