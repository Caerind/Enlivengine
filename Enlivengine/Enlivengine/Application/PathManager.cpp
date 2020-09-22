#include <Enlivengine/Application/PathManager.hpp>

#include <filesystem>
#include <Enlivengine/System/Assert.hpp>

namespace en
{

PathManager::PathManager()
	: mExecutablePath("")
	, mScreenshotPath("")
{
}

void PathManager::SetExecutablePath(const char* executablePath)
{
	if (executablePath != nullptr)
	{
		mExecutablePath = executablePath;
		std::filesystem::path cleanExePath = mExecutablePath;
		mExecutablePath = cleanExePath.parent_path().generic_string();
	}
}

const std::string& PathManager::GetAssetsPath() const
{
#ifdef ENLIVE_DEBUG
	static bool assetsPathDefined = false;
	static std::string assetsPath = "";

	if (!assetsPathDefined)
	{
		std::filesystem::path closeAssetsPath = std::filesystem::path(mExecutablePath + "/Assets");
		std::filesystem::path farAssetsPath = std::filesystem::path(mExecutablePath + "/../../../Assets");
		if (std::filesystem::exists(closeAssetsPath))
		{
			assetsPath = "Assets/";
		}
		else if (std::filesystem::exists(farAssetsPath))
		{
			assetsPath = "../../Assets/";
		}

		assetsPathDefined = true;
	}
#else
	static const std::string assetsPath = "Assets/";
#endif // ENLIVE_DEBUG

	return assetsPath;
}

const std::string& PathManager::GetAssetsPathAbsolute() const
{
	static bool initialized = false;
	static std::string assetsPathAbsolute;
	if (!initialized)
	{
		std::filesystem::path p = GetAssetsPath();
		if (!p.is_absolute())
		{
			p = std::filesystem::absolute(p);
		}
		assetsPathAbsolute = p.string();
		std::replace(assetsPathAbsolute.begin(), assetsPathAbsolute.end(), '\\', '/');
	}
	return assetsPathAbsolute;
}

const std::string& PathManager::GetFontsPath() const
{
	static std::string fontsPath = GetAssetsPath() + "Fonts/";
	return fontsPath;
}

const std::string& PathManager::GetMapsPath() const
{
	static std::string mapsPath = GetAssetsPath() + "Maps/";
	return mapsPath;
}

const std::string& PathManager::GetMusicsPath() const
{
	static std::string musicsPath = GetAssetsPath() + "Musics/";
	return musicsPath;
}

const std::string& PathManager::GetSoundsPath() const
{
	static std::string soundsPath = GetAssetsPath() + "Sounds/";
	return soundsPath;
}

const std::string& PathManager::GetTexturesPath() const
{
	static std::string texturesPath = GetAssetsPath() + "Textures/";
	return texturesPath;
}

void PathManager::SetScreenshotPath(const std::string& screenshotPath)
{
	mScreenshotPath = screenshotPath;
}

const std::string& PathManager::GetScreenshotPath() const
{
	return mScreenshotPath;
}

} // namespace en
