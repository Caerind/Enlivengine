#pragma once

namespace en
{
	
class Debugger
{
public:
	Debugger() = delete;
	
	static bool IsPresent();
	static void Break();

	// TODO : Create Breakpoints with features : Togglables/Conditions/Actions

	// TODO : Create DataBreakpoints (using dbg-tools)

};
	
} // namespace en