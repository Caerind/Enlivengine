#include <Enlivengine/Platform/Filesystem.hpp>

namespace en
{

bool Exists(const Path& path)
{
#if defined(ENLIVE_PLATFORM_MINGW) && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
	return std::experimental::filesystem::exists(path);
#else
	return std::filesystem::exists(path);
#endif // ENLIVE_PLATFORM_MINGW && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
}

bool Remove(const Path& path)
{
#if defined(ENLIVE_PLATFORM_MINGW) && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
	return std::experimental::filesystem::remove(path);
#else
	return std::filesystem::remove(path);
#endif // ENLIVE_PLATFORM_MINGW && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
}

Path Absolute(const Path& path)
{
#if defined(ENLIVE_PLATFORM_MINGW) && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
	return std::experimental::filesystem::absolute(path);
#else
	return std::filesystem::absolute(path);
#endif // ENLIVE_PLATFORM_MINGW && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
}

} // namespace en