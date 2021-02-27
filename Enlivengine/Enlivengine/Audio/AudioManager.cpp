#include <Enlivengine/Audio/AudioManager.hpp>

namespace en
{

AudioManager::AudioManager()
	: mEngine()
{
}

bool AudioManager::Initialize()
{
	return mEngine.init() == 0;
}

bool AudioManager::Release()
{
	mEngine.deinit();
	return true;
}

void AudioManager::PlaySource(AudioSource& source)
{
	if (source.mSource != nullptr)
	{
		mEngine.play(*(source.mSource));
	}
}

} // namespace en