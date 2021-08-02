#include <Enlivengine/Audio/AudioManager.hpp>

#include <Enlivengine/Utils/Log.hpp>

namespace en
{

AudioManager::AudioManager()
	: mEngine()
{
}

bool AudioManager::Initialize()
{
	// Initialize SoLoud
	const auto result = mEngine.init();
	if (result == 0)
	{
		enLogInfo(LogChannel::Global, "SoLoud initialized");
		return true;
	}
	else
	{
		enLogError(LogChannel::Global, "Can't initialize SoLoud (error {})", result);
		return false;
	}
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