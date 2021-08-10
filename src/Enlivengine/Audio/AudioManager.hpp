#pragma once

#include <soloud.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>
#include <Enlivengine/Audio/AudioStatus.hpp>
#include <Enlivengine/Audio/AudioSource.hpp>

namespace en
{

class AudioManager
{
	ENLIVE_SINGLETON(AudioManager);

public:
	bool Initialize();
	bool Release();
	bool IsInitialized() const;
	
	void PlaySource(AudioSource& source);

private:
	SoLoud::Soloud mEngine;
	bool mInitialized;
};

} // namespace en