#include <Enlivengine/Audio/MusicManager.hpp>

namespace en
{

MusicManager::MusicManager()
	: mMusicsVolume(1.0f)
	, mMusicsEnabled(true)
	, mMusicFilenames()
	, mMusics()
{
}

F32 MusicManager::GetMusicVolume() const
{
	return mMusicsVolume;
}

void MusicManager::SetMusicVolume(F32 volume)
{
	mMusicsVolume = volume;
	UpdateMusicsVolume();
}

bool MusicManager::AreMusicsEnabled() const
{
	return mMusicsEnabled;
}

void MusicManager::SetMusicsEnabled(bool enabled)
{
	mMusicsEnabled = enabled;
	UpdateMusicsVolume();
}

MusicID MusicManager::PrepareMusic(const char* id, const std::string& filename)
{
	ENLIVE_UNUSED(id);
	ENLIVE_UNUSED(filename);
	/*
	// TODO : AUDIO UPDATE
	MusicID index(ResourceManager::StringToResourceID(id));
	if (mMusicFilenames.find(index) == mMusicFilenames.end())
	{
		mMusicFilenames[index] = filename;
	}
	return index;
	*/
	return InvalidMusicID;
}

MusicPtr MusicManager::PlayMusic(MusicID id, bool loop /*= true*/)
{
	ENLIVE_UNUSED(id);
	ENLIVE_UNUSED(loop);
	/*
	// TODO : AUDIO UPDATE
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
	*/
	return MusicPtr();
}

MusicPtr MusicManager::PlayMusic(const char* id, bool loop /*= true*/)
{
	ENLIVE_UNUSED(id);
	ENLIVE_UNUSED(loop);
	/*
	// TODO : AUDIO UPDATE
	return PlayMusic(ResourceManager::StringToResourceID(id), loop);
	*/
	return MusicPtr();
}

F32 MusicManager::GetCurrentMusicsVolume() const
{
	if (!mMusicsEnabled)
	{
		return 0.0f;
	}
	else
	{
		return mMusicsVolume;
	}
}

U32 MusicManager::GetCurrentMusicsCount() const
{
	return static_cast<U32>(mMusics.size());
}

void MusicManager::Play()
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMusics[i]->Play();
	}
}

void MusicManager::Pause()
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMusics[i]->Pause();
	}
}

void MusicManager::Stop()
{
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		delete mMusics[i];
		mMusics[i] = nullptr;
	}
	mMusics.clear();
}

void MusicManager::Update()
{
	size_t musicsSize = mMusics.size();
	for (size_t i = 0; i < musicsSize; )
	{
		if (mMusics[i]->GetStatus() == AudioStatus::Stopped)
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
}

void MusicManager::ForceStopMusic(U32 musicUID)
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

Music* MusicManager::GetMusicInternal(U32 musicUID)
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

const Music* MusicManager::GetMusicInternal(U32 musicUID) const
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

void MusicManager::UpdateMusicsVolume()
{
	const F32 currentVolume = GetCurrentMusicsVolume();
	const size_t size = mMusics.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMusics[i]->UpdateManagerVolume(currentVolume);
	}
}

} // namespace en