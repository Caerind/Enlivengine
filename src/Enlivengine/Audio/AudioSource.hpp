#pragma once

#include <Enlivengine/Audio/SoloudIncl.hpp>

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