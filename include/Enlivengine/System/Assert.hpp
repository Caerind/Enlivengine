#pragma once

#include <Enlivengine/System/Config.hpp>
#include <Enlivengine/System/Log.hpp>

#ifdef assert
#undef assert
#endif
#ifdef ENLIVE_ENABLE_ASSERT
	#define assert(expr) \
		if (expr) {} \
		else \
		{ \
			::en::LogManager::write(en::LogType::Error, en::LogChannel::All, 10, "Assertion failed!\nExpr : %s\n File : %s\n Line : %d\n", #expr, __FILE__, __LINE__); \
			for (;;) {} \
		}
#else
	#define assert
#endif