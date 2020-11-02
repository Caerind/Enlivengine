#include <Enlivengine/Audio/Music.hpp>

#ifdef ENLIVE_MODULE_AUDIO

#include <Enlivengine/Audio/MusicManager.hpp>

namespace en
{

U32 Music::sMusicUIDGenerator = 0;

/*
// TODO : AUDIO UPDATE
Music::Music(MusicID musicID, const std::string& filename)
	: mMusic()
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
*/

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
	ENLIVE_UNUSED(loop);
	// TODO : AUDIO UPDATE
	//mMusic.setLoop(loop);
}

bool Music::IsLoop() const
{
	// TODO : AUDIO UPDATE
	//return mMusic.getLoop();
	return true;
}

void Music::SetVolume(F32 userVolume)
{
	mUserVolume = userVolume;
	// TODO : AUDIO UPDATE
	//mMusic.setVolume(userVolume * AudioSystem::GetInstance().GetCurrentMusicsVolume() * 100.0f);
}

F32 Music::GetVolume() const
{
	return mUserVolume;
}

F32 Music::GetFinalVolume() const
{
	// TODO : AUDIO UPDATE
	//return 0.01f * mMusic.getVolume();
	return 1.0f;
}

void Music::Play()
{
	// TODO : AUDIO UPDATE
	//mMusic.play();
	mStatus = AudioStatus::Playing;
}

void Music::Pause()
{
	// TODO : AUDIO UPDATE
	//mMusic.pause();
	mStatus = AudioStatus::Paused;
}

void Music::Stop()
{
	// TODO : AUDIO UPDATE
	//mMusic.stop();
	mStatus = AudioStatus::Stopped;
	MusicManager::GetInstance().ForceStopMusic(mMusicUID);
}

AudioStatus Music::GetStatus() const
{
	return mStatus;
}

void Music::UpdateManagerVolume(F32 managerVolume)
{
	ENLIVE_UNUSED(managerVolume);
	// TODO : AUDIO UPDATE
	//mMusic.setVolume(managerVolume * mUserVolume * 100.0f);
}

MusicPtr::MusicPtr(MusicID musicID, U32 musicUID)
	: mMusicID(musicID)
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

AudioStatus MusicPtr::GetStatus() const
{
	if (const Music* music = GetMusic())
	{
		return music->GetStatus();
	}
	return AudioStatus::Stopped;
}

Music* MusicPtr::GetMusic()
{
	if (mMusicID != InvalidMusicID && mMusicUID != InvalidMusicUID)
	{
		return MusicManager::GetInstance().GetMusicInternal(mMusicUID);
	}
	return nullptr;
}

const Music* MusicPtr::GetMusic() const
{
	if (mMusicID != InvalidMusicID && mMusicUID != InvalidMusicUID)
	{
		return MusicManager::GetInstance().GetMusicInternal(mMusicUID);
	}
	return nullptr;
}

} // namespace en

#endif // ENLIVE_MODULE_AUDIO