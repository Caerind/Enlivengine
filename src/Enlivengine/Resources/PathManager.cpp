#include <Enlivengine/Resources/PathManager.hpp>

#include <filesystem>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

const std::string& PathManager::GetCurrentPath()
{
	static std::string path = "";
	if (path.size() == 0)
	{
		path = std::filesystem::current_path().generic_string();
		path.append("/");
	}
	return path;
}

void PathManager::SetExecutablePath(const char* executablePath)
{
	PathManager& instance = GetInstance();
	if (executablePath != nullptr && strlen(executablePath) > 0)
	{
		instance.mExecutablePath = executablePath;
		const std::filesystem::path cleanExePath = instance.mExecutablePath;
		instance.mExecutablePath = cleanExePath.parent_path().generic_string();
		instance.mExecutablePath.append("/");
	}
	else
	{
		instance.mExecutablePath = "";
	}
}

const std::string& PathManager::GetExecutablePath()
{
	return GetInstance().mExecutablePath;
}

bool PathManager::AutoDetectAssetsPath()
{
	constexpr U32 kScanDepth = 5;

	std::string backfolder = "";
	for (U32 i = 0; i < kScanDepth; ++i)
	{
		if (i > 0)
		{
			backfolder += "../";
		}

		std::filesystem::path tempAssetsPath = std::filesystem::path(GetCurrentPath() + backfolder + "Assets").lexically_normal();
		if (std::filesystem::exists(tempAssetsPath))
		{
			SetAssetsPath(tempAssetsPath.generic_string() + "/");
			return true;
		}
	}
	
#if defined(ENLIVE_TOOL)
	// Start backward to get the examples at the root instead of the one in the build folder
	for (I32 i = static_cast<I32>(kScanDepth); i > 0; --i) 
	{
		if (i < 5)
		{
			backfolder.resize(backfolder.size() - 3);
		}

		std::filesystem::path tempCurrentExamplesPath = std::filesystem::path(GetCurrentPath() + backfolder + "examples").lexically_normal();
		if (std::filesystem::exists(tempCurrentExamplesPath))
		{
			SetAssetsPath(tempCurrentExamplesPath.generic_string() + "/");
			return true;
		}

		std::filesystem::path tempExecutableExamplesPath = std::filesystem::path(GetExecutablePath() + backfolder + "examples").lexically_normal();
		if (std::filesystem::exists(tempExecutableExamplesPath))
		{
			SetAssetsPath(tempExecutableExamplesPath.generic_string() + "/");
			return true;
		}
	}
#endif // ENLIVE_TOOL 

	return false;
}

void PathManager::SetAssetsPath(const std::string& assetPath)
{
	GetInstance().mAssetsPath = assetPath;
}

const std::string& PathManager::GetAssetsPath()
{
	return GetInstance().mAssetsPath;
}

bool PathManager::AutoDetectTmpPath()
{
	for (U32 i = 0; i < 5; ++i)
	{
		std::string backfolder = "";
		for (U32 j = 0; j < i; ++j)
		{
			backfolder += "../";
		}

		std::filesystem::path tempAssetsPath = std::filesystem::path(GetCurrentPath() + backfolder + "tmp").lexically_normal();
		if (std::filesystem::exists(tempAssetsPath))
		{
			SetTmpPath(tempAssetsPath.generic_string() + "/");
			return true;
		}
	}

#if defined(ENLIVE_RELEASE)
	std::filesystem::path tempAssetsPath = std::filesystem::current_path().generic_string();
	if (std::filesystem::exists(tempAssetsPath))
	{
		SetTmpPath(tempAssetsPath.generic_string() + "/");
		return true;
	}
#endif // ENLIVE_RELEASE 

	return false;
}

void PathManager::SetTmpPath(const std::string& tmpPath)
{
	GetInstance().mTmpPath = tmpPath;
}

const std::string& PathManager::GetTmpPath()
{
	return GetInstance().mTmpPath;
}

void PathManager::SetScreenshotPath(const std::string& screenshotPath)
{
	GetInstance().mScreenshotPath = screenshotPath;
}

const std::string& PathManager::GetScreenshotPath()
{
	return GetInstance().mScreenshotPath;
}

PathManager& PathManager::GetInstance()
{
	static PathManager instance;
	return instance;
}

} // namespace en