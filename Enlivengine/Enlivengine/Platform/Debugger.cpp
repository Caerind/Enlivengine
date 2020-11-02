#include <Enlivengine/Platform/Debugger.hpp>

#ifdef ENLIVE_MODULE_PLATFORM

#include <Enlivengine/Platform/CompilerTraits.hpp>
#include <Enlivengine/Platform/PlatformTraits.hpp>

#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
#include <Windows.h>
#include <debug_break/debugbreak.h>
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)

namespace en
{

bool Debugger::IsPresent()
{
#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
	return IsDebuggerPresent();
#else
	return false;
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
}

void Debugger::Break()
{
#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
	debug_break();
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
}
	
} // namespace en

#endif // ENLIVE_MODULE_PLATFORM