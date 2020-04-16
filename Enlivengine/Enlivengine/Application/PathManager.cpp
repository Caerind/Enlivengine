#include <Enlivengine/Application/PathManager.hpp>

namespace en
{

PathManager::PathManager()
	: mScreenshotPath("")
{
}

const std::string& PathManager::GetAssetsPath() const
{
#ifdef ENLIVE_DEBUG
	static std::string assetsPath = "../../Assets/";
#else
	static std::string assetsPath = "Assets/";
#endif // ENLIVE_DEBUG
	return assetsPath;
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
