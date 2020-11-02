#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_RESOURCES

#include <string>

#ifdef ENLIVE_MODULE_GRAPHICS
#include <bgfx/bgfx.h>
#endif // ENLIVE_MODULE_GRAPHICS

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>

namespace en
{

class PathManager
{
public:
	static void SetExecutablePath(const char* executablePath);
	static const std::string& GetExecutablePath();

	static bool AutoDetectAssetsPath();
	static void SetAssetsPath(const std::string& assetsPath);
	static const std::string& GetAssetsPath();

	static bool AutoDetectShadersPath();
	static void SetShadersPath(const std::string& shadersPath);
	static const std::string& GetShadersPath();

#ifdef ENLIVE_MODULE_GRAPHICS
	static const std::string& GetShadersPathForRenderer(bgfx::RendererType::Enum renderer);
#endif // ENLIVE_MODULE_GRAPHICS

	static void SetScreenshotPath(const std::string& screenshotPath);
	static const std::string& GetScreenshotPath();

private:
	static PathManager& GetInstance();

	PathManager();

	// NonCopyable & NonMovable
	PathManager(const PathManager&) = delete;
	PathManager& operator=(const PathManager&) = delete;
	PathManager(PathManager&&) = delete;
	PathManager& operator=(PathManager&&) = delete;

	std::string mExecutablePath;
	std::string mAssetsPath;
	std::string mShadersPath;
#ifdef ENLIVE_MODULE_GRAPHICS
	std::string mShadersPathRenderer;
#endif // ENLIVE_MODULE_GRAPHICS
	std::string mScreenshotPath;
};

} // namespace en

#endif // ENLIVE_MODULE_RESOURCES