#include <Enlivengine/Core/Engine.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Profiler.hpp>
#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/SDL/SDLWrapper.hpp>
#include <Enlivengine/Graphics/BgfxWrapper.hpp>
#include <Enlivengine/Graphics/Camera.hpp>
#include <Enlivengine/Tools/ImGuiToolManager.hpp>
#include <Enlivengine/Window/EventSystem.hpp>

#include <Enlivengine/Graphics/DebugDraw.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Tilemap.hpp>

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
#include <Enlivengine/Window/Keyboard.hpp>
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

#ifdef ENLIVE_TOOL
#include <Enlivengine/Tools/ImGuiEditor.hpp>
#include <Enlivengine/Tools/ImGuiGame.hpp>
#endif // ENLIVE_TOOL

#include <filesystem>

namespace en
{

int Engine::Main(int argc, char** argv)
{
	if (Engine::Init(argc, argv))
	{
		Time dt;
		while (Engine::Update(dt))
		{
			if (World* world = Engine::GetCurrentWorld())
			{
				// Update
				{
					world->Update(dt);

#ifdef ENLIVE_TOOL
					if (ImGuiEditor::IsViewVisible())
					{
						ImGuiEditor::UpdateCamera(dt);
					}
#endif // ENLIVE_TOOL
				}


				// Render
				{
#ifdef ENLIVE_TOOL
					if (ImGuiGame::IsViewVisible())
					{
						if (Camera* mainCamera = Camera::GetMainCamera())
						{
							mainCamera->Apply();
							world->Render();
							world->GetDebugDraw().Render();
						}
					}

					if (ImGuiEditor::IsViewVisible())
					{
						ImGuiEditor::GetCamera().Apply();
						world->Render();
						world->GetDebugDraw().Render();
					}
					world->GetDebugDraw().Clear();
#endif // ENLIVE_TOOL

#ifdef ENLIVE_RELEASE
					if (Camera* mainCamera = Camera::GetMainCamera())
					{
						mainCamera->Apply();
						world->Render();
#ifdef ENLIVE_DEBUG
						world->GetDebugDraw().Render();
						world->GetDebugDraw().Clear();
#endif // ENLIVE_DEBUG
					}
#endif // ENLIVE_RELEASE
				}
			}

			BgfxWrapper::Frame();
		}

		if (World* world = Engine::GetCurrentWorld())
		{
			world->GetEntityManager().ClearEntities();
		}

		ResourceManager::GetInstance().ReleaseAll();
	}
	Engine::Release();
	return 0;
}

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
		if (ImGuiToolManager::Initialize())
		{
#ifdef ENLIVE_TOOL
			if (!ImGuiToolManager::LoadFromFile(PathManager::GetAssetsPath() + "tools.json"))
			{
				enLogWarning(LogChannel::Core, "Can't load ImGui opened tools");
			}
#endif // ENLIVE_TOOL
		}
		else
		{
#ifdef ENLIVE_TOOL
			enLogError(LogChannel::Core, "Can't initialize ImGuiToolManager");
			return false;
#else
			enLogWarning(LogChannel::Core, "Can't initialize ImGuiToolManager");
#endif // ENLIVE_TOOL
		}
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

	engine.mInitialized = false;

#ifdef ENLIVE_ENABLE_IMGUI
	if (ImGuiWrapper::IsInitialized())
	{
		if (ImGuiToolManager::IsInitialized())
		{
#ifdef ENLIVE_TOOL
			ImGuiToolManager::SaveToFile(PathManager::GetAssetsPath() + "tools.json");
#endif // ENLIVE_TOOL
			ImGuiToolManager::Release();
		}

		ImGuiWrapper::Release();
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

#ifdef ENLIVE_ENABLE_PROFILE
	static U32 frameNumber = 0;

	static bool firstTime = true;
	if (!firstTime)
	{
		Profiler::GetInstance().EndFrame();
		frameNumber++;
	}
	firstTime = false;

	Profiler::GetInstance().StartFrame(frameNumber);
#endif // ENLIVE_ENABLE_PROFILE

	if (!engine.mWindow.ShouldClose())
	{
		dt = engine.mDTClock.Restart();

		EventSystem::Update();

#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiToolManager::Update(engine.mWindow, dt);
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

bool Engine::CreateWorld(const std::string& worldName)
{
	auto& engine = GetInstance();

	if (engine.mCurrentWorld != nullptr)
	{
		UnloadCurrentWorld();
	}

	enAssert(engine.mCurrentWorld == nullptr);

	engine.mCurrentWorld = new World(worldName);
	return engine.mCurrentWorld->SaveToFile();
}

bool Engine::RemoveWorld(const std::string& worldName)
{
	auto& engine = GetInstance();

	if (engine.mCurrentWorld != nullptr && engine.mCurrentWorld->GetName() == worldName)
	{
		enLogError(LogChannel::Core, "Can't remove the currently loaded world {}", engine.mCurrentWorld->GetName());
		return false;
	}

	return std::filesystem::remove(std::filesystem::path(World::GetWorldFilename(worldName)));
}

bool Engine::LoadWorld(const std::string& worldName)
{
	auto& engine = GetInstance();

	if (engine.mCurrentWorld != nullptr)
	{
		UnloadCurrentWorld();
	}

	enAssert(engine.mCurrentWorld == nullptr);

	engine.mCurrentWorld = new World(worldName);
	return engine.mCurrentWorld->LoadFromFile();
}

void Engine::UnloadCurrentWorld()
{
	auto& engine = GetInstance();

	enAssert(engine.mCurrentWorld != nullptr);

	delete engine.mCurrentWorld;
	engine.mCurrentWorld = nullptr;
}

World* Engine::GetCurrentWorld()
{
	return GetInstance().mCurrentWorld;
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