#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_UTILS

namespace en
{

class NonCopyable
{
protected:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};

} // namespace en

#endif // ENLIVE_MODULE_UTILS