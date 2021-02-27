#pragma once

#include <soloud.h>

namespace en
{

class AudioSource
{
public:
	AudioSource();

protected:
	friend class AudioManager;
	SoLoud::AudioSource* mSource;
};

} // namespace en