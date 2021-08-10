#include <Enlivengine/Audio/AudioManager.hpp>

#include <Enlivengine/Utils/Log.hpp>

namespace en
{

AudioManager::AudioManager()
	: mEngine()
	, mInitialized(false)
{
}

bool AudioManager::Initialize()
{
	// Initialize SoLoud
	const auto result = mEngine.init();
	if (result == SoLoud::SO_NO_ERROR)
	{
		mInitialized = true;
		enLogInfo(LogChannel::Global, "SoLoud initialized");
		return true;
	}
	else
	{
		mInitialized = false;
		enLogError(LogChannel::Global, "Can't initialize SoLoud (error {})", Enum::GetValueName<SoLoud::SOLOUD_ERRORS>(static_cast<SoLoud::SOLOUD_ERRORS>(result)));
		return false;
	}
}

bool AudioManager::Release()
{
	mInitialized = false;
	mEngine.deinit();
	return true;
}

void AudioManager::PlaySource(AudioSource& source)
{
	if (source.mSource != nullptr && mInitialized)
	{
		mEngine.play(*(source.mSource));
	}
}

} // namespace en