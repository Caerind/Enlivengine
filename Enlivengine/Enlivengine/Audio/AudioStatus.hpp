#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_AUDIO

namespace en
{

enum class AudioStatus
{
	Playing,
	Paused,
	Stopped
};

} // namespace en

#endif // ENLIVE_MODULE_AUDIO