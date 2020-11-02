#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_PLATFORM

namespace en
{
	
class Debugger
{
public:
	Debugger() = delete;
	
	static bool IsPresent();
	static void Break();
};
	
} // namespace en

#endif // ENLIVE_MODULE_PLATFORM