#include <Enlivengine/Resources/PathManager.hpp>

#include <Enlivengine/Platform/Filesystem.hpp>
#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

void PathManager::SetExecutablePath(const char* executablePath)
{
	PathManager& instance = GetInstance();
	if (executablePath != nullptr)
	{
		instance.mExecutablePath = executablePath;
		const Path cleanExePath = instance.mExecutablePath;
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

	for (U32 i = 0; i < 5; ++i)
	{
		std::string backfolder = "";
		for (U32 j = 0; j < i; ++j)
		{
			backfolder += "/..";
		}
		Path tempAssetsPath = Path(instance.mExecutablePath + backfolder + "/Assets");
		if (Exists(tempAssetsPath))
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

#if defined(ENLIVE_RELEASE)
	Path tempAssetsPath = Path(instance.mExecutablePath + "/Assets");
	if (Exists(tempAssetsPath))
	{
		SetAssetsPath("Assets/");
		return true;
	}
#endif // ENLIVE_RELEASE 

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

	for (U32 i = 0; i < 5; ++i)
	{
		std::string backfolder = "";
		for (U32 j = 0; j < i; ++j)
		{
			backfolder += "/..";
		}
		Path tempShadersPath = Path(instance.mExecutablePath + backfolder + "/Shaders");
		if (Exists(tempShadersPath))
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

#if defined(ENLIVE_RELEASE)
	Path tempAssetsPath = Path(instance.mExecutablePath + "/Assets/Shaders");
	if (Exists(tempAssetsPath))
	{
		SetShadersPath("Assets/Shaders/");
		return true;
	}
#endif // ENLIVE_RELEASE

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

std::string PathManager::GetAbsolutePath(const std::string& path)
{
	Path p = path;
	if (!p.is_absolute())
	{
		p = Absolute(p);
	}
	std::string result = p.string();
	std::replace(result.begin(), result.end(), '\\', '/');
	return result;
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