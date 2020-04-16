#include <Enlivengine/System/Debugger.hpp>

#include <debug_break/debugbreak.h>
//#include <dbgtools/debugger.h>

namespace en
{

bool Debugger::IsPresent()
{
	//return debugger_present() > 0;
	return true;
}

void Debugger::Break()
{
	debug_break();
}
	
} // namespace en
