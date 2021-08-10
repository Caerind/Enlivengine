#include <Enlivengine/Engine.hpp>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Utils/Profiler.hpp>
#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/SDL/SDLWrapper.hpp>
#include <Enlivengine/Graphics/BgfxWrapper.hpp>
#include <Enlivengine/Graphics/Camera.hpp>
#include <Enlivengine/Tools/ImGuiToolManager.hpp>
#include <Enlivengine/Tools/ImGuiEditor.hpp>
#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Engine/WorldFileManager.hpp>

#include <Enlivengine/Audio/AudioManager.hpp>

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
#include <Enlivengine/Window/Keyboard.hpp>
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

#include <Enlivengine/Graphics/DebugDraw.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Tilemap.hpp>

#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/UIDComponent.hpp>
#include <Enlivengine/Core/CameraComponent.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/PhysicComponent2D.hpp>
#include <Enlivengine/Core/PhysicSystem2D.hpp>

namespace en
{

int Engine::Main(int argc, char** argv)
{
	// Components
	RegisterComponent<NameComponent>();
	RegisterComponent<UIDComponent>();
	RegisterComponent<RenderableComponent>();
	RegisterComponent<SpriteComponent>();
	RegisterComponent<TilemapComponent>();
	RegisterComponent<CameraComponent>();
	RegisterComponent<TransformComponent>();
	RegisterComponent<PhysicComponent2D>();

	// Systems
	RegisterSystem<PhysicSystem2D>();

	if (Engine::Init(argc, argv))
	{
		while (Engine::Update())
		{
			World* world = Universe::GetCurrentWorld();
			if (world != nullptr)
			{
#ifdef ENLIVE_TOOL
				if (ImGuiEditor::IsPlaying())
#endif // ENLIVE_TOOL
				{
					world->UpdatePhysic();
					world->Update();
				}
#ifdef ENLIVE_TOOL
				else
				{
					world->UpdateTool();
				}
#endif // ENLIVE_TOOL
			}

			if (world != nullptr && world->GetSystemManager().HasRenderSystem())
			{
				world->Render();
			}
			else
			{
#ifdef ENLIVE_TOOL
				Framebuffer& framebuffer = ImGuiEditor::GetFramebuffer();
#else
				Framebuffer& framebuffer = Framebuffer::GetDefaultFramebuffer();
#endif // ENLIVE_TOOL
				BgfxWrapper::ClearFramebuffer(framebuffer, Colors::DarkGray);
        
				// TODO : Add text describing the issue if any
      }

			BgfxWrapper::Frame();
		}

		if (Universe::GetCurrentWorld() != nullptr)
		{
			WorldFileManager::UnloadCurrentWorld();
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
		enLogWarning(LogChannel::Global, "No arg[0] for Engine::Init, using empty executable path");
		PathManager::SetExecutablePath("");
	}
	enLogInfo(LogChannel::Global, "ExecutablePath: {}", PathManager::GetExecutablePath());
	enLogInfo(LogChannel::Global, "CurrentPath: {}", PathManager::GetCurrentPath());

	const bool assetsPathFound = PathManager::AutoDetectAssetsPath();
	if (!assetsPathFound)
	{
		enLogError(LogChannel::Global, "Can't find AssetsPath");
		return false;
	}
	enLogInfo(LogChannel::Global, "AssetsPath: {}", PathManager::GetAssetsPath());

	const bool tmpPathFound = PathManager::AutoDetectTmpPath();
	if (!tmpPathFound)
	{
		enLogError(LogChannel::Global, "Can't find TmpPath");
		return false;
	}
	enLogInfo(LogChannel::Global, "TmpPath: {}", PathManager::GetTmpPath());

	if (!SDLWrapper::Init())
	{
		return false;
	}

	AudioManager::GetInstance().Initialize();

	const bool windowCreated = engine.mWindow.Create("Enlivengine", 0);
	if (!windowCreated)
	{
		enLogError(LogChannel::Global, "Can't create window");
		return false;
	}
	enLogInfo(LogChannel::Global, "Window created");

	if (!BgfxWrapper::Init(engine.mWindow))
	{
		enLogError(LogChannel::Global, "Can't initialize Bgfx");
		return false;
	}
	enLogInfo(LogChannel::Global, "Bgfx initialized");
	enLogInfo(LogChannel::Global, "Renderer: {}", Enum::GetValueName<bgfx::RendererType::Enum>(bgfx::getRendererType()));

	const bool shadersPathFound = PathManager::AutoDetectShadersPath();
	if (!shadersPathFound)
	{
		enLogError(LogChannel::Global, "Can't find ShadersPath");
		return false;
	}
	enLogInfo(LogChannel::Global, "ShadersPath: {}", PathManager::GetShadersPathForRenderer(bgfx::getRendererType()));

	const bool spriteInit = Sprite::InitializeSprites();
	const bool tilemapInit = Tilemap::InitializeTilemaps();
	const bool debugDrawInit = DebugDraw::InitializeDebugDraws();
	if (!spriteInit || !tilemapInit || !debugDrawInit)
	{
		enLogError(LogChannel::Global, "Can't initialize graphics resources");
		return false;
	}
	enLogInfo(LogChannel::Global, "Shaders initialized");

#ifdef ENLIVE_ENABLE_IMGUI
	const std::string imGuiToolsFilePath = PathManager::GetTmpPath() + "tools.json";
#ifdef ENLIVE_TOOL
	if (!std::filesystem::exists(imGuiToolsFilePath))
	{
		ImGuiToolManager::CreateDefaultToolsFile(imGuiToolsFilePath);
	}
#endif // ENLIVE_TOOL
	if (ImGuiToolManager::Initialize())
	{
#ifdef ENLIVE_TOOL
		ImGuiToolManager::LoadFromFile(imGuiToolsFilePath);
#endif // ENLIVE_TOOL
		enLogInfo(LogChannel::Global, "ImGui initialized");
	}
	else
	{
#ifdef ENLIVE_TOOL
		enLogError(LogChannel::Global, "Can't initialize ImGuiToolManager");
		return false;
#else
		enLogWarning(LogChannel::Global, "Can't initialize ImGuiToolManager");
#endif // ENLIVE_TOOL
	}

	const std::string imGuiIniFilePath = PathManager::GetTmpPath() + "imgui.ini";
#ifdef ENLIVE_TOOL
	if (!std::filesystem::exists(imGuiIniFilePath))
	{
		ImGuiToolManager::CreateDefaultImGuiFile(imGuiIniFilePath);
	}
#endif // ENLIVE_TOOL
	if (!ImGuiWrapper::Init(imGuiIniFilePath))
	{
		return false;
	}
#endif // ENLIVE_ENABLE_IMGUI

	engine.mInitialized = true;
	enLogInfo(LogChannel::Global, "Engine initialized");

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
			ImGuiToolManager::SaveToFile(PathManager::GetTmpPath() + "tools.json");
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

	AudioManager::GetInstance().Release();

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

bool Engine::Update()
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
		Time::SetDeltaTime(engine.mDTClock.Restart());

		EventSystem::Update();

#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiToolManager::Update(engine.mWindow);
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