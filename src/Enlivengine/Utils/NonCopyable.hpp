#pragma once

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