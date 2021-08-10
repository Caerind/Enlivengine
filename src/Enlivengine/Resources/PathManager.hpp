#pragma once

#include <string>

#include <bgfx/bgfx.h>

#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Singleton.hpp>

namespace en
{

class PathManager
{
public:
	static const std::string& GetCurrentPath();

	static void SetExecutablePath(const char* executablePath);
	static const std::string& GetExecutablePath();

	static bool AutoDetectAssetsPath();
	static void SetAssetsPath(const std::string& assetsPath);
	static const std::string& GetAssetsPath();

	static bool AutoDetectTmpPath();
	static void SetTmpPath(const std::string& tmpPath);
	static const std::string& GetTmpPath();

	static bool AutoDetectShadersPath();
	static void SetShadersPath(const std::string& shadersPath);
	static const std::string& GetShadersPath();

	static const std::string& GetShadersPathForRenderer(bgfx::RendererType::Enum renderer);

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
	std::string mTmpPath;
	std::string mShadersPath;
	std::string mShadersPathRenderer;
	std::string mScreenshotPath;
};

} // namespace en