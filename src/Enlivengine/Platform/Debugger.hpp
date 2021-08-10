#pragma once

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