#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/SDL/SDLWrapper.hpp>
#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Window/Controller.hpp>
#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Keyboard.hpp>
#include <Enlivengine/Graphics/BgfxWrapper.hpp>
#include <Enlivengine/Graphics/ImGuiWrapper.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Tilemap.hpp>
#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/Tools/ImGuiToolManager.hpp>
#include <Enlivengine/Tools/ImGuiDemoWindow.hpp>
#include <Enlivengine/Tools/ImGuiEntityBrowser.hpp>
#include <Enlivengine/Tools/ImGuiInputEditor.hpp>
#include <Enlivengine/Tools/ImGuiConsole.hpp>
#include <Enlivengine/Tools/ImGuiAnimationEditor.hpp>
#include <Enlivengine/Tools/ImGuiLogger.hpp>
#include <Enlivengine/Tools/ImGuiPhysic.hpp>
#include <Enlivengine/Tools/ImGuiMemoryDebugger.hpp>
#include <Enlivengine/Tools/ImGuiProfiler.hpp>
#include <Enlivengine/Tools/ImGuiResourceBrowser.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/ComponentManager.hpp>
#include <Enlivengine/Core/TraceryGenerator.hpp>

#include <Enlivengine/Meta/MetaSpecialization.hpp>

using namespace en;

template <std::size_t N>
inline std::vector<std::string> arrayStringViewToVectorString(const std::array<std::string_view, N>& arr)
{
	std::vector<std::string> vec;
	vec.reserve(arr.size());
	for (const auto& value : arr)
	{
		vec.push_back(std::string(value));
	}
	return vec;
}

class RenderSystem : public System
{
public:
	RenderSystem(World& world) : System(world) {}

	void Render() override
	{
		const bgfx::ViewId renderViewId = 0;
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<RenderableComponent>();
		for (auto entt : view)
		{
			Entity entity(entityManager, entt);
			if (entity)
			{
				if (entity.Has<TransformComponent>())
				{
					bgfx::setTransform(entity.Get<TransformComponent>().transform.GetMatrix().GetData());
					mWorld.GetDebugDraw().DrawTransform(entity.Get<TransformComponent>().transform.GetMatrix());
				}
				if (entity.Has<SpriteComponent>())
				{
					entity.Get<SpriteComponent>().sprite.Render(renderViewId);
				}
			}
		}
		mWorld.GetDebugDraw().Render(renderViewId);
	}
};

void FPSCamera(Camera& camera, F32 dtSeconds, F32 forwardMvt, F32 leftMvt, F32 deltaYaw, F32 deltaPitch)
{
	Vector3f direction = camera.GetRotation().GetForward();

	// Movement
	if (forwardMvt != 0.0f || leftMvt != 0.0f)
	{
		Vector3f mvtUnit = direction;
		mvtUnit.y = 0.0f;
		mvtUnit.Normalize();

		Vector3f movement;
		movement += 3.0f * forwardMvt * mvtUnit * dtSeconds;
		movement -= 3.0f * leftMvt * mvtUnit.CrossProduct(ENLIVE_DEFAULT_UP) * dtSeconds;
		camera.Move(movement);
	}

	// Rotation
	if (deltaYaw != 0.0f || deltaPitch != 0.0f)
	{
		if (!Math::Equals(deltaYaw, 0.0f))
		{
			camera.Rotate(Quaternionf(100.0f * dtSeconds * deltaYaw, ENLIVE_DEFAULT_UP));
		}
		if (!Math::Equals(deltaPitch, 0.0f))
		{
			//camera.Rotate(Quaternionf(100.0f * dtSeconds * deltaPitch, direction.CrossProduct(ENLIVE_DEFAULT_UP)));
		}
	}
}

int main(int argc, char** argv)
{
	if (argc >= 1)
	{
		PathManager::SetExecutablePath(argv[0]);
		const bool assetsPathFound = PathManager::AutoDetectAssetsPath();
		enAssert(assetsPathFound);
		const bool shadersPathFound = PathManager::AutoDetectShadersPath();
		enAssert(shadersPathFound);
	}

	SDLWrapper::Init();
	{
		Window window;
		window.Create("Test", 800, 600);
		BgfxWrapper::Init(window);
#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiWrapper::Init();
#endif // ENLIVE_ENABLE_IMGUI
		Sprite::InitializeSprites();
		Tilemap::InitializeTilemaps();
		DebugDraw::InitializeDebugDraws();

		ImGuiToolManager::GetInstance().Initialize();
		ImGuiDemoWindow::GetInstance().Register();
		ImGuiEntityBrowser::GetInstance().Register();
		ImGuiAnimationEditor::GetInstance().Register();
		ImGuiConsole::GetInstance().Register();
		ImGuiInputEditor::GetInstance().Register();
		ImGuiLogger::GetInstance().Register();
		ImGuiMemoryDebugger::GetInstance().Register();
		ImGuiPhysic::GetInstance().Register();
		ImGuiProfiler::GetInstance().Register();
		ImGuiResourceBrowser::GetInstance().Register();
		{
			ComponentManager::Register<NameComponent>();
			ComponentManager::Register<TransformComponent>();
			ComponentManager::Register<UIDComponent>();
			ComponentManager::Register<RenderableComponent>();
			ComponentManager::Register<SpriteComponent>();
			ComponentManager::Register<CameraComponent>();

			printf("%d joysticks, %d haptics\n", Controller::GetJoystickCount(), Controller::GetHapticCount());
			for (U32 i = 0; i < Controller::GetJoystickCount(); ++i)
			{
				Controller::Rumble(i, 0.25f, 100); // Ensure the controller is working when debugging
				printf("%s name\n%d axes, %d balls, %d buttons, %d hats\n", Controller::GetName(i), Controller::GetAxisCount(i), Controller::GetBallCount(i), Controller::GetButtonCount(i), Controller::GetHatCount(i));
				printf("%d haptic\n", Controller::IsHaptic(i));
				printf("\n");
			}

			const char* textureAFilename = "fieldstone-rgba.dds";
			TexturePtr textureA = ResourceManager::GetInstance().Create<Texture>("textureA", TextureLoader::FromFile(PathManager::GetAssetsPath() + textureAFilename));
			if (!textureA.IsValid())
			{
				enAssert(false);
			}

			const char* textureBFilename = "ship_default.png";
			TexturePtr textureB = ResourceManager::GetInstance().Create<Texture>("textureB", TextureLoader::FromFile(PathManager::GetAssetsPath() + textureBFilename));
			if (!textureB.IsValid())
			{
				enAssert(false);
			}

			enum class PetColors
			{
				Orange,
				Blue,
				White,
				Black,
				Grey,
				Purple,
				Indigo,
				Turquoise
			};
			TraceryGenerator tracery;
			tracery.AddEnglishModifiers();
			tracery.AddModifier("lowercase", [](const std::string& str)
				{
					std::string ret;
					ret.resize(str.size());
					for (std::size_t i = 0; i < str.size(); ++i)
					{
						ret[i] = tolower(str[i]);
					}
					return ret;
				});
			tracery.AddModifier("uppercase", [](const std::string& str)
				{
					std::string ret;
					ret.resize(str.size());
					for (std::size_t i = 0; i < str.size(); ++i)
					{
						ret[i] = toupper(str[i]);
					}
					return ret;
				});
			tracery.AddGrammar("sentence", { "The #color.lowercase# #animal# is called #name.uppercase# and has a \"#texture#\" texture" });
			tracery.AddGrammar("color", arrayStringViewToVectorString(Enum::GetValueNames<PetColors>()));
			tracery.AddGrammar("animal", { "unicorn","raven","sparrow","scorpion","coyote","eagle","owl","lizard","zebra","duck","kitten" });
			tracery.AddGrammar("name", { "Arjun","Yuuma","Darcy","Mia","Chiaki","Izzi","Azra","Lina" });
			tracery.AddGrammar("texture", { textureAFilename, textureBFilename });
			std::string traceryString = tracery.Generate("#sentence#");

			World world;
			world.CreateSystem<RenderSystem>();
			Universe::GetInstance().SetCurrentWorld(&world);

			Entity playerEntity = world.GetEntityManager().CreateEntity();
			playerEntity.Add<NameComponent>().name = "Player";
			playerEntity.Add<TransformComponent>().transform.SetPosition(Vector3f(0.0f, 0.0f, 2.0f));
			Camera& playerCam = playerEntity.Add<CameraComponent>().camera;
			playerCam.InitializePerspective(80.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f);
			playerCam.InitializeView({ 0.0f, 0.8f, 0.0f }, Quaternionf(0.0f, ENLIVE_DEFAULT_FORWARD));

			bool useFreeCamera = false;
			world.GetFreeCamera().InitializePerspective(80.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f);
			world.GetFreeCamera().InitializeView({ 0.0f, 0.8f, 0.0f }, Quaternionf(0.0f, ENLIVE_DEFAULT_FORWARD));
			enSlotType(Window, OnResized) cameraWindowResize;
			cameraWindowResize.Connect(window.OnResized, [&world, &playerCam](const Window*, U32 width, U32 height)
				{
					world.GetFreeCamera().InitializePerspective(80.0f, F32(width) / F32(height), 0.1f, 100.0f);
					playerCam.InitializePerspective(80.0f, F32(width) / F32(height), 0.1f, 100.0f);
				});

			Transform* b1Transform = nullptr; // You should not keep things that way, but it is a simple example

			{
				Entity a1 = world.GetEntityManager().CreateEntity();
				a1.Add<NameComponent>().name = "A1";
				a1.Add<TransformComponent>().transform.SetPosition(Vector3f(0.0f, 1.0f, 0.0f));
				a1.Add<RenderableComponent>();
				a1.Add<SpriteComponent>().sprite.SetTexture(textureA);

				Entity a2 = world.GetEntityManager().CreateEntity();
				a2.Add<NameComponent>().name = "A2";
				a2.Add<TransformComponent>().transform.SetPosition(Vector3f(1.0f, 2.0f, 0.0f));
				a2.Add<RenderableComponent>();
				a2.Add<SpriteComponent>().sprite.SetTexture(textureA);

				Entity b1 = world.GetEntityManager().CreateEntity();
				b1.Add<NameComponent>().name = "B1";
				b1Transform = &b1.Add<TransformComponent>().transform;
				b1Transform->SetPosition(Vector3f(2.0f, 2.0f, 0.0f));
				b1.Add<RenderableComponent>();
				b1.Add<SpriteComponent>().sprite.SetTexture(textureB);
			}

			Tileset tileset;
			tileset.SetGridSize({ 2,2 });
			tileset.SetTileSize({ 256, 256 });
			tileset.SetTexture(textureA);
			Tilemap tilemap;
			tilemap.SetTileset(tileset);
			tilemap.SetSize({ 4,4 });
			tilemap.SetTile({ 1,1 }, 0);
			tilemap.SetTile({ 2,1 }, 1);
			tilemap.SetTile({ 2,2 }, 2);
			tilemap.SetTile({ 1,2 }, 3);
			Matrix4f tilemapTransform = Matrix4f::RotationX(90.0f);

			const bgfx::ViewId mainViewId = 0;
			const bgfx::ViewId imguiViewId = 250;

			// Create button event using generic way 
			EventSystem::AddButton("moveForward", EventSystem::EventButton::Type::KeyboardKey, static_cast<U32>(Keyboard::Key::W), static_cast<U32>(Keyboard::Modifier::None), EventSystem::EventButton::ActionType::Hold);
			// Create button event using specific helpers
			EventSystem::AddKeyButton("moveLeft", Keyboard::Key::A, EventSystem::EventButton::ActionType::Hold);
			EventSystem::AddKeyButton("moveBackward", Keyboard::Key::S, EventSystem::EventButton::ActionType::Hold);
			EventSystem::AddKeyButton("moveRight", Keyboard::Key::D, EventSystem::EventButton::ActionType::Hold);
			EventSystem::AddKeyButton("action", Keyboard::Key::E, EventSystem::EventButton::ActionType::Pressed);
			EventSystem::AddJoystickButton("jactionP1", 0, 0, EventSystem::EventButton::ActionType::Pressed);
			EventSystem::AddJoystickButton("jactionP2", 1, 0, EventSystem::EventButton::ActionType::Pressed);
			// Keep the hash for faster lookups (and no error in the strings)
			const U32 toggleGraphStats = EventSystem::AddKeyButton("toggleGraphStats", Keyboard::Key::F3, EventSystem::EventButton::ActionType::Pressed, static_cast<U32>(Keyboard::Modifier::Control));

			Clock clock;

			while (!window.ShouldClose())
			{
				EventSystem::Update();

#ifdef ENLIVE_ENABLE_IMGUI
				ImGuiWrapper::BeginFrame(250, window.GetWidth(), window.GetHeight());

				ImGuiToolManager::GetInstance().Update();

#ifdef ENLIVE_DEBUG
				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				const auto& selectedEntities = world.GetSelectedEntities();
				for (const auto& enttEntity : selectedEntities)
				{
					Entity entity(world, enttEntity);
					if (entity && entity.Has<TransformComponent>())
					{
						ImGuizmo::Manipulate(
							useFreeCamera ? world.GetFreeCamera().GetViewMatrix().GetData() : playerCam.GetViewMatrix().GetData(),
							useFreeCamera ? world.GetFreeCamera().GetProjectionMatrix().GetData() : playerCam.GetProjectionMatrix().GetData(),
							ImGuizmo::TRANSLATE,
							ImGuizmo::WORLD,
							entity.Get<TransformComponent>().transform.GetMatrix().GetData()
						);
					}
				}
#endif // ENLIVE_DEBUG

				ImGuiWrapper::EndFrame();
#endif // ENLIVE_ENABLE_IMGUI

				const Time dt = clock.Restart();

				if (EventSystem::IsButtonActive("action") || EventSystem::IsButtonActive("jactionP1"))
				{
					printf("Action!\n");
					if (!Controller::Rumble(0, 0.25f, 100))
					{
						printf("Can't rumble : %s\n", SDLWrapper::GetError());
					}
				}

				if (b1Transform != nullptr)
				{
					b1Transform->GetMatrix().ApplyRotation(Matrix3f::RotationY(180.0f * dt.AsSeconds()));
				}

				if (Keyboard::IsPressed(Keyboard::Key::Escape))
				{
					window.Close();
				}
				if (Keyboard::IsPressed(Keyboard::Key::Space))
				{
					useFreeCamera = !useFreeCamera;
				}

				// Camera movement
				if (Keyboard::IsAltHold())
				{
					Mouse::SetRelativeMode(true);

					F32 forward = 0.0f;
					F32 left = 0.0;
					if (Keyboard::IsHold(Keyboard::Key::W)) forward += 1.0f;
					if (Keyboard::IsHold(Keyboard::Key::S)) forward -= 1.0f;
					if (Keyboard::IsHold(Keyboard::Key::A)) left += 1.0f;
					if (Keyboard::IsHold(Keyboard::Key::D)) left -= 1.0f;
					const F32 deltaYaw = -Mouse::GetMouseMovement().x * 0.25f;
					const F32 deltaPitch = Mouse::GetMouseMovement().y * 0.15f;
					FPSCamera(world.GetFreeCamera(), dt.AsSeconds(), forward, left, deltaYaw, deltaPitch);
				}
				else
				{
					Mouse::SetRelativeMode(false);
				}

				// Move player using controller
				FPSCamera(playerCam, dt.AsSeconds(), -Controller::GetAxis(0, 1), -Controller::GetAxis(0, 0), -Controller::GetAxis(0, 3), Controller::GetAxis(0, 4));

				// Toggle debug stats
#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
				if (EventSystem::IsButtonActive(toggleGraphStats))
				{
					BgfxWrapper::ToggleDisplayStats();
				}
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

				world.GetDebugDraw().DrawCross(Vector3f(-1.0f));
				world.GetDebugDraw().DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);
				world.GetDebugDraw().DrawSphere({ -1.0f, 0.5f, -3.0f }, 0.5f, Colors::Red);
				if (useFreeCamera)
				{
					world.GetDebugDraw().DrawFrustum(playerCam.CreateFrustum(), Colors::Blue);
					world.GetDebugDraw().DrawTransform(Matrix4f::Transform(playerCam.GetPosition(), playerCam.GetRotation()));
				}
				else
				{
					world.GetDebugDraw().DrawFrustum(world.GetFreeCamera().CreateFrustum(), Colors::Green);
					world.GetDebugDraw().DrawTransform(Matrix4f::Transform(world.GetFreeCamera().GetPosition(), world.GetFreeCamera().GetRotation()));
				}
				world.GetDebugDraw().DrawGrid(Vector3f::Zero(), ENLIVE_DEFAULT_UP, -10, 10, 1, Colors::White);

				// Render
				{
					bgfx::touch(mainViewId);

					// Display mouse pos
					bgfx::dbgTextClear();
					/*
					const Vector2i dbgMousePos = Mouse::GetPositionCurrentWindow();
					const Vector2i dbgMouseDeltaPos = Mouse::GetMouseMovement();
					const I32 dbgMouseWheel = Mouse::GetWheel();
					bgfx::dbgTextPrintf(0, 0, 0x0f, "Mouse: (%d, %d) (%d, %d) (%d)", dbgMousePos.x, dbgMousePos.y, dbgMouseDeltaPos.x, dbgMouseDeltaPos.y, dbgMouseWheel);
					*/

					bgfx::dbgTextPrintf(0, 3, 0x0f, "%s", traceryString.c_str());

					if (useFreeCamera)
					{
						const Vector3f position = world.GetFreeCamera().GetPosition();
						bgfx::dbgTextPrintf(0, 2, 0x0f, "Position: (%f, %f, %f)", position.x, position.y, position.z);
						world.GetFreeCamera().Apply(mainViewId);
					}
					else
					{
						const Vector3f position = playerCam.GetPosition();
						bgfx::dbgTextPrintf(0, 2, 0x0f, "Position: (%f, %f, %f)", position.x, position.y, position.z);
						playerCam.Apply(mainViewId);
					}

					world.Render();

					bgfx::setTransform(tilemapTransform.GetData());
					tilemap.Render(mainViewId);

					bgfx::frame();
				}
			}

			world.GetEntityManager().ClearEntities();

			textureA.ReleaseFromManager();
			textureB.ReleaseFromManager();
		}
		ImGuiResourceBrowser::GetInstance().Unregister();
		ImGuiProfiler::GetInstance().Unregister();
		ImGuiPhysic::GetInstance().Unregister();
		ImGuiMemoryDebugger::GetInstance().Unregister();
		ImGuiLogger::GetInstance().Unregister();
		ImGuiInputEditor::GetInstance().Unregister();
		ImGuiConsole::GetInstance().Unregister();
		ImGuiAnimationEditor::GetInstance().Unregister();
		ImGuiEntityBrowser::GetInstance().Unregister();
		ImGuiDemoWindow::GetInstance().Unregister();
		ImGuiToolManager::GetInstance().Shutdown();

		DebugDraw::ReleaseDebugDraws();
		Tilemap::ReleaseTilemaps();
		Sprite::ReleaseSprites();
#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiWrapper::Release();
#endif // ENLIVE_ENABLE_IMGUI
		BgfxWrapper::Release();
	}
	SDLWrapper::Release();
	return 0;
}
