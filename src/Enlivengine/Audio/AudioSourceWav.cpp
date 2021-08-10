#include <Enlivengine/Audio/AudioSourceWav.hpp>

#include <soloud_wav.h>

namespace en
{

AudioSourceWav::AudioSourceWav()
{
	mSource = new SoLoud::Wav();
}

AudioSourceWav::AudioSourceWav(AudioSourceWav&& other)
{
	mSource = other.mSource;
	other.mSource = nullptr;
}

AudioSourceWav::~AudioSourceWav()
{
	if (mSource != nullptr)
	{
		mSource->stop();
		delete mSource;
		mSource = nullptr;
	}
}

AudioSourceWav& AudioSourceWav::operator=(AudioSourceWav&& other)
{
	if (this != &other)
	{
		mSource = other.mSource;
		other.mSource = nullptr;
	}
	return *this;
}

bool AudioSourceWav::Load(const char* filename)
{
	if (mSource != nullptr)
	{
		return static_cast<SoLoud::Wav*>(mSource)->load(filename) == 0;
	}
	else
	{
		return false;
	}
}

} // namespace en