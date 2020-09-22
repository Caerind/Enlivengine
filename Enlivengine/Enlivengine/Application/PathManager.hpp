#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Singleton.hpp>

#include <string>

namespace en
{

class PathManager
{
	ENLIVE_SINGLETON(PathManager);

public:	
	void SetExecutablePath(const char* executablePath);

	const std::string& GetAssetsPath() const;
	const std::string& GetAssetsPathAbsolute() const;
	
	const std::string& GetFontsPath() const;
	const std::string& GetMapsPath() const;
	const std::string& GetMusicsPath() const;
	const std::string& GetSoundsPath() const;
	const std::string& GetTexturesPath() const;

	void SetScreenshotPath(const std::string& screenshotPath);
	const std::string& GetScreenshotPath() const;

private:
	std::string mExecutablePath;
	std::string mScreenshotPath;
};

} // namespace en
