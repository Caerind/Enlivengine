#include <Enlivengine/Application/AudioSystem.hpp>

#include <Enlivengine/System/Hash.hpp>

namespace en
{

AudioSystem::AudioSystem()
	: mStatus(sf::SoundSource::Playing)
	, mMusicVolume(100.0f)
	, mSoundVolume(100.0f)
{
}

MusicId AudioSystem::createMusic(const char* id, const std::string& filename)
{
	MusicId index(Hash::CRC32(id));
	if (mMusicFilenames.find(index) == mMusicFilenames.end())
	{
		mMusicFilenames[index] = filename;
	}
	return index;
}

AudioSystem::MusicPtr AudioSystem::playMusic(MusicId id, bool loop)
{
    if (mStatus != sf::SoundSource::Stopped && mMusicFilenames.find(id) != mMusicFilenames.end() && mMusics.size() < MAX_MUSIC)
    {
		MusicPtr m(std::make_shared<sf::Music>());
		mMusics.push_back(m);
		m->openFromFile(mMusicFilenames[id]);
        m->setLoop(loop);
        m->setVolume(mMusicVolume);
        m->play();
        if (mStatus == sf::SoundSource::Paused)
        {
            m->pause();
        }
        return m;
    }
    return nullptr;
}

AudioSystem::MusicPtr AudioSystem::playMusic(const char* id, bool loop)
{
	return playMusic(Hash::CRC32(id), loop);
}

SoundId AudioSystem::createSound(const char* id, const std::string& filename)
{
	return mSoundBuffers.create(id, SoundBufferLoader::loadFromFile(filename));
}

AudioSystem::SoundPtr AudioSystem::playSound(SoundId id)
{
	if (mStatus != sf::SoundSource::Stopped && mSoundBuffers.has(id) && mSounds.size() < MAX_SOUND)
    {
		SoundPtr s(std::make_shared<sf::Sound>());
        mSounds.push_back(s);
        s->setBuffer(mSoundBuffers.get(id));
        s->setVolume(mSoundVolume);
        s->play();
        if (mStatus == sf::SoundSource::Paused)
        {
            s->pause();
        }
        return s;
    }
	return nullptr;
}

AudioSystem::SoundPtr AudioSystem::playSound(const char* id)
{
	return playSound(Hash::CRC32(id));
}

void AudioSystem::play()
{
    if (mStatus == sf::SoundSource::Paused)
    {
        update();
        for (auto itr = mMusics.begin(); itr != mMusics.end(); ++itr)
        {
            (*itr)->play();
        }
        for (auto itr = mSounds.begin(); itr != mSounds.end(); ++itr)
        {
            (*itr)->play();
        }
        mStatus = sf::SoundSource::Playing;
    }
}

void AudioSystem::pause()
{
    if (mStatus == sf::SoundSource::Playing)
    {
        update();
        for (auto itr = mMusics.begin(); itr != mMusics.end(); ++itr)
        {
            (*itr)->pause();
        }
        for (auto itr = mSounds.begin(); itr != mSounds.end(); ++itr)
        {
            (*itr)->pause();
        }
        mStatus = sf::SoundSource::Paused;
    }
}

void AudioSystem::stop()
{
    if (mStatus != sf::SoundSource::Stopped)
    {
        mMusics.clear();
        mSounds.clear();
    }
}

void AudioSystem::update()
{
    for (auto itr = mMusics.begin(); itr != mMusics.end();)
    {
        if (mStatus != sf::SoundSource::Stopped && (*itr)->getStatus() == sf::SoundSource::Stopped)
        {
            itr = mMusics.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
    for (auto itr = mSounds.begin(); itr != mSounds.end();)
    {
        if (mStatus != sf::SoundSource::Stopped && (*itr)->getStatus() == sf::SoundSource::Stopped)
        {
            itr = mSounds.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}

void AudioSystem::setGlobalVolume(F32 volume)
{
    sf::Listener::setGlobalVolume(volume);
}

void AudioSystem::setMusicVolume(F32 volume)
{
    mMusicVolume = volume;
    for (auto itr = mMusics.begin(); itr != mMusics.end(); ++itr)
    {
        (*itr)->setVolume(volume);
    }
}

void AudioSystem::setSoundVolume(F32 volume)
{
    mSoundVolume = volume;
    for (auto itr = mSounds.begin(); itr != mSounds.end(); ++itr)
    {
        (*itr)->setVolume(volume);
    }
}

F32 AudioSystem::getGlobalVolume() const
{
	return sf::Listener::getGlobalVolume();
}

F32 AudioSystem::getMusicVolume() const
{
    return mMusicVolume;
}

F32 AudioSystem::getSoundVolume() const
{
    return mSoundVolume;
}

sf::SoundSource::Status AudioSystem::getStatus() const
{
    return mStatus;
}

} // namespace en
