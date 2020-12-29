#pragma once

#include <Enlivengine/Platform/PrimitiveTypes.hpp>

#if defined(ENLIVE_PLATFORM_MINGW) && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
#include <experimental/filesystem>
#else
#include <filesystem>
#endif // ENLIVE_PLATFORM_MINGW && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8

namespace en
{

#if defined(ENLIVE_PLATFORM_MINGW) && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8
using Path = std::experimental::filesystem::path;
#else
using Path = std::filesystem::path;
#endif // ENLIVE_PLATFORM_MINGW && ENLIVE_VERSION_DECODE_MAJOR(ENLIVE_COMPILER_VERSION) <= 8

bool Exists(const Path& path);
bool Remove(const Path& path);
Path Absolute(const Path& path);

} // namespace en