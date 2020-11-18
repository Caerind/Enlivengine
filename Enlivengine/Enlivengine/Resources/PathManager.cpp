#include <Enlivengine/Resources/PathManager.hpp>

#include <filesystem>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

void PathManager::SetExecutablePath(const char* executablePath)
{
	PathManager& instance = GetInstance();
	if (executablePath != nullptr)
	{
		instance.mExecutablePath = executablePath;
		const std::filesystem::path cleanExePath = instance.mExecutablePath;
		instance.mExecutablePath = cleanExePath.parent_path().generic_string();
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
	PathManager& instance = GetInstance();
#ifdef ENLIVE_TOOL
	for (U32 i = 0; i < 5; ++i)
	{
		std::string backfolder = "";
		for (U32 j = 0; j < i; ++j)
		{
			backfolder += "/..";
		}
		std::filesystem::path tempAssetsPath = std::filesystem::path(instance.mExecutablePath + backfolder + "/Assets");
		if (std::filesystem::exists(tempAssetsPath))
		{
			if (i == 0)
			{
				SetAssetsPath("Assets/");
			}
			else
			{
				SetAssetsPath(backfolder.substr(4) + "/Assets/");
			}
			return true;
		}
	}
#else
	std::filesystem::path tempAssetsPath = std::filesystem::path(instance.mExecutablePath + "/Assets");
	if (std::filesystem::exists(tempAssetsPath))
	{
		SetAssetsPath("Assets/");
		return true;
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

bool PathManager::AutoDetectShadersPath()
{
	PathManager& instance = GetInstance();
#ifdef ENLIVE_TOOL
	for (U32 i = 0; i < 5; ++i)
	{
		std::string backfolder = "";
		for (U32 j = 0; j < i; ++j)
		{
			backfolder += "/..";
		}
		std::filesystem::path tempShadersPath = std::filesystem::path(instance.mExecutablePath + backfolder + "/Shaders");
		if (std::filesystem::exists(tempShadersPath))
		{
			if (i == 0)
			{
				SetShadersPath("Shaders/");
				instance.mShadersPath = "Shaders/";
			}
			else
			{
				SetShadersPath(backfolder.substr(4) + "/Shaders/");
			}
			return true;
		}
	}
#else
	std::filesystem::path tempAssetsPath = std::filesystem::path(instance.mExecutablePath + "/Assets/Shaders");
	if (std::filesystem::exists(tempAssetsPath))
	{
		SetShadersPath("Assets/Shaders/");
		return true;
	}
#endif // ENLIVE_TOOL
	return false;
}

void PathManager::SetShadersPath(const std::string& shadersPath)
{
	GetInstance().mShadersPath = shadersPath;
	GetInstance().mShadersPathRenderer.clear();
}

const std::string& PathManager::GetShadersPath()
{
	return GetInstance().mShadersPath;
}

const std::string& PathManager::GetShadersPathForRenderer(bgfx::RendererType::Enum renderer)
{
	PathManager& instance = GetInstance();
	if (instance.mShadersPathRenderer.size() > 0)
	{
		return instance.mShadersPathRenderer;
	}
	else
	{
		switch (renderer)
		{
		case bgfx::RendererType::Noop:
			return instance.mShadersPathRenderer;
			break;
		case bgfx::RendererType::Direct3D9:
			instance.mShadersPathRenderer = PathManager::GetShadersPath() + "dx9/";
			break;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12:
			instance.mShadersPathRenderer = PathManager::GetShadersPath() + "dx11/";
			break;
		case bgfx::RendererType::Gnm:
			instance.mShadersPathRenderer = PathManager::GetShadersPath() + "pssl/";
			break;
		case bgfx::RendererType::Metal:
			instance.mShadersPathRenderer = PathManager::GetShadersPath() + "metal/";
			break;
		case bgfx::RendererType::OpenGL:
			instance.mShadersPathRenderer = PathManager::GetShadersPath() + "glsl/";
			break;
		case bgfx::RendererType::OpenGLES:
			instance.mShadersPathRenderer = PathManager::GetShadersPath() + "essl/";
			break;
		case bgfx::RendererType::Vulkan:
			instance.mShadersPathRenderer = PathManager::GetShadersPath() + "spirv/";
			break;
		default:
			enAssert(false); // Unhandled renderer type
			break;
		}
		return instance.mShadersPathRenderer;
	}
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

PathManager::PathManager()
	: mExecutablePath("")
	, mAssetsPath("")
	, mShadersPath("")
	, mShadersPathRenderer("")
	, mScreenshotPath("")
{
}

} // namespace en