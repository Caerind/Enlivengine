#include <Enlivengine/System/Debugger.hpp>

#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/PlatformTraits.hpp>

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
