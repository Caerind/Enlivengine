#include <Enlivengine/Core/Engine.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/SDL/SDLWrapper.hpp>
#include <Enlivengine/Graphics/BgfxWrapper.hpp>
#include <Enlivengine/Tools/ImGuiToolManager.hpp>
#include <Enlivengine/Window/EventSystem.hpp>

#include <Enlivengine/Graphics/DebugDraw.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Tilemap.hpp>

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
#include <Enlivengine/Window/Keyboard.hpp>
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

namespace en
{

bool Engine::Init(int argc, char** argv)
{
	Engine& engine = GetInstance();
	enAssert(!engine.mInitialized);

	if (argc >= 1)
	{
		PathManager::SetExecutablePath(argv[0]);
	}
	else
	{
		enLogWarning(LogChannel::Core, "No arg[0] for Engine::Init, using empty executable path");
		PathManager::SetExecutablePath("");
	}
	const bool assetsPathFound = PathManager::AutoDetectAssetsPath();
	if (!assetsPathFound)
	{
		enLogError(LogChannel::Core, "Can't find AssetsPath");
		return false;
	}
	const bool shadersPathFound = PathManager::AutoDetectShadersPath();
	if (!shadersPathFound)
	{
		enLogError(LogChannel::Core, "Can't find ShadersPath");
		return false;
	}

	if (!SDLWrapper::Init())
	{
		enLogError(LogChannel::Core, "Can't initialize SDL");
		return false;
	}

	const bool windowCreated = engine.mWindow.Create("Enlivengine", 0);
	if (!windowCreated)
	{
		enLogError(LogChannel::Core, "Can't create window");
		return false;
	}

	if (!BgfxWrapper::Init(engine.mWindow))
	{
		enLogError(LogChannel::Core, "Can't initialize Bgfx");
		return false;
	}

	const bool spriteInit = Sprite::InitializeSprites();
	const bool tilemapInit = Tilemap::InitializeTilemaps();
	const bool debugDrawInit = DebugDraw::InitializeDebugDraws();
	if (!spriteInit || !tilemapInit || !debugDrawInit)
	{
		enLogError(LogChannel::Core, "Can't initialize graphics resources");
		return false;
	}

#ifdef ENLIVE_ENABLE_IMGUI
	if (ImGuiWrapper::Init(PathManager::GetAssetsPath() + "imgui.ini"))
	{
		ImGuiToolManager::GetInstance().Initialize();
#ifdef ENLIVE_TOOL
		ImGuiToolManager::GetInstance().LoadFromFile(PathManager::GetAssetsPath() + "tools.json");
#endif // ENLIVE_TOOL
	}
#endif // ENLIVE_ENABLE_IMGUI

	engine.mInitialized = true;

	return true;
}

bool Engine::IsInitialized()
{
	return GetInstance().mInitialized;
}

bool Engine::Release()
{
	Engine& engine = GetInstance();
	enAssert(engine.mInitialized);

	engine.mInitialized = false;

#ifdef ENLIVE_ENABLE_IMGUI
	if (ImGuiWrapper::IsInitialized())
	{
#ifdef ENLIVE_TOOL
		ImGuiToolManager::GetInstance().SaveToFile(PathManager::GetAssetsPath() + "tools.json");
#endif // ENLIVE_TOOL
		ImGuiToolManager::GetInstance().Release();
	}
#endif // ENLIVE_ENABLE_IMGUI

	DebugDraw::ReleaseDebugDraws();
	Tilemap::ReleaseTilemaps();
	Sprite::ReleaseSprites();

	if (BgfxWrapper::IsInitialized())
	{
		BgfxWrapper::Release();
	}

	engine.mWindow.Close();
	engine.mWindow.Destroy();

	if (SDLWrapper::IsInitialized())
	{
		SDLWrapper::Release();
	}

	return true;
}

Window& Engine::GetWindow()
{
	return GetInstance().mWindow;
}

bool Engine::Update(Time& dt)
{
	Engine& engine = GetInstance();
	enAssert(engine.mInitialized);

	if (!engine.mWindow.ShouldClose())
	{
		dt = engine.mDTClock.Restart();

		EventSystem::Update();

#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiToolManager::GetInstance().Update(engine.mWindow, dt);
#endif // ENLIVE_ENABLE_IMGUI

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
		if (Keyboard::IsPressed(Keyboard::Key::F3) && Keyboard::IsControlHold())
		{
			BgfxWrapper::ToggleDisplayStats();
		}
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

		return true;
	}
	else
	{
		return false;
	}
}

Engine& Engine::GetInstance()
{
	static Engine instance;
	return instance;
}

Engine::Engine()
	: mInitialized(false)
{
}

Engine::~Engine()
{
	enAssert(!mInitialized);
}

} // namespace en