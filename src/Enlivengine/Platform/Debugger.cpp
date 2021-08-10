#include <Enlivengine/Platform/Debugger.hpp>

#include <Enlivengine/Config.hpp>

#include <Enlivengine/Platform/CompilerTraits.hpp>
#include <Enlivengine/Platform/PlatformTraits.hpp>

#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)
#include <Windows.h>
#include <debug_break/debugbreak.h>
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)

namespace en
{

bool Debugger::IsPresent()
{
#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)
	return IsDebuggerPresent();
#else
	return false;
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)
}

void Debugger::Break()
{
#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)
	debug_break();
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC) && defined(ENLIVE_DEBUG)
}
	
} // namespace en