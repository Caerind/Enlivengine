#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_UTILS

#include <Enlivengine/Platform/Debugger.hpp>
#include <Enlivengine/Utils/Log.hpp>

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

#endif // ENLIVE_MODULE_UTILS