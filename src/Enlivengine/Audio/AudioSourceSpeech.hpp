#pragma once

#include <string>

#include <soloud.h>

#include <Enlivengine/Audio/AudioSource.hpp>

namespace en
{

class AudioSourceSpeech : public AudioSource
{
public:
	AudioSourceSpeech();
	AudioSourceSpeech(const AudioSourceSpeech& other) = delete;
	AudioSourceSpeech(AudioSourceSpeech&& other);
	~AudioSourceSpeech();

	AudioSourceSpeech& operator=(const AudioSourceSpeech& other) = delete;
	AudioSourceSpeech& operator=(AudioSourceSpeech&& other);

	bool SetText(const std::string& text);
	const std::string& GetText() const;

private:
	std::string mText;
};

} // namespace en