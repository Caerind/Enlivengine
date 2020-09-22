#pragma once

#include <Enlivengine/Config.hpp>
#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/Debugger.hpp>

#ifdef ENLIVE_ENABLE_ASSERT
	#define enAssertChannel(expr, channel) \
		if (expr) {} \
		else \
		{ \
			enLogFatal(channel, "Assertion failed!\nExpr : {}\nFile : {}\nLine : {}\n", #expr, __FILE__, __LINE__); \
			::en::Debugger::Break(); \
		}
	#define enAssert(expr) enAssertChannel(expr, en::LogChannel::Global) 
#else
	#define enAssertChannel(expr, channel) 
	#define enAssert(expr) 
#endif