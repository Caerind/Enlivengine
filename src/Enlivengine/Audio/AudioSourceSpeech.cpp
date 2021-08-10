#include <Enlivengine/Audio/AudioSourceSpeech.hpp>

#include <soloud_speech.h>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

AudioSourceSpeech::AudioSourceSpeech()
	: AudioSource()
	, mText("")
{
	mSource = new SoLoud::Speech();
}

AudioSourceSpeech::AudioSourceSpeech(AudioSourceSpeech&& other)
	: AudioSource()
{
	mSource = other.mSource;
	mText = other.mText;
	other.mSource = nullptr;
	other.mText = "";
}

AudioSourceSpeech::~AudioSourceSpeech()
{
	if (mSource != nullptr)
	{
		mSource->stop();
		delete mSource;
		mSource = nullptr;
	}
}

AudioSourceSpeech& AudioSourceSpeech::operator=(AudioSourceSpeech&& other)
{
	if (this != &other)
	{
		mSource = other.mSource;
		mText = other.mText;
		other.mSource = nullptr;
		other.mText = "";
	}
	return *this;
}

bool AudioSourceSpeech::SetText(const std::string& text)
{
	enAssert(mSource != nullptr); 
	const bool result = static_cast<SoLoud::Speech*>(mSource)->setText(text.c_str()) == 0;
	if (result)
	{
		mText = text;
	}
	return result;
}

const std::string& AudioSourceSpeech::GetText() const
{
	return mText;
}

} // namespace en