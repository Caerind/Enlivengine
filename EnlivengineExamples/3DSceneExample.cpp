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
#include <Enlivengine/Graphics/DebugDraw.hpp>
#include <Enlivengine/Graphics/Camera.hpp>
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

#include <Enlivengine/Meta/MetaSpecialization.hpp>

using namespace en;

class RenderSystem : public System
{
public:
	RenderSystem(World& world) : System(world) {}

	void Render() override
	{
		const bgfx::ViewId renderViewId = 0;
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<RenderableComponent, SpriteComponent>();
		for (auto entt : view)
		{
			Entity entity(entityManager, entt);
			if (entity)
			{
				if (entity.Has<TransformComponent>())
				{
					bgfx::setTransform(entity.Get<TransformComponent>().transform.GetMatrix().GetData());
				}
				entity.Get<SpriteComponent>().sprite.Render(renderViewId);
			}
		}
	}
};

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

			


			World world;
			world.CreateSystem<RenderSystem>();
			Universe::GetInstance().SetCurrentWorld(&world);


			{
				Entity a1 = world.GetEntityManager().CreateEntity();
				a1.Add<NameComponent>().name = "A1";
				a1.Add<TransformComponent>().transform.SetPosition(0.0f, 1.0f, 0.0f);
				a1.Add<RenderableComponent>();
				a1.Add<SpriteComponent>().sprite.SetTexture(textureA);

				Entity a2 = world.GetEntityManager().CreateEntity();
				a2.Add<NameComponent>().name = "A2";
				a2.Add<TransformComponent>().transform.SetPosition(1.0f, 2.0f, 0.0f);
				a2.Add<RenderableComponent>();
				a2.Add<SpriteComponent>().sprite.SetTexture(textureA);
			}

			Sprite spriteB;
			spriteB.SetTexture(textureB);
			Matrix4f spriteBTransform = Matrix4f::Translation(2.0f, 2.0f, 0.0f);

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

			Camera camera;
			camera.InitializePerspective(80.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f);
			//camera.InitializeOrthographic(-10.0f, 10.0f, 10.0f, -10.0f, 0.01f, 100.0f);
			camera.InitializeView(Vector3f(0.0f, 1.0f, 5.0f), Vector3f(0.0f, 0.0f, -1.0f));

			// TODO : Fix me
			Frustum frustum = camera.CreateFrustum();
			std::vector<Vector3f> pointsToTest;
			pointsToTest.push_back(Vector3f::Zero());
			pointsToTest.push_back(Vector3f::UnitX());
			pointsToTest.push_back(Vector3f::UnitY());
			pointsToTest.push_back(Vector3f::UnitZ());
			pointsToTest.push_back(Vector3f(10, 0, 0));
			pointsToTest.push_back(Vector3f(-10, 0, 0));
			pointsToTest.push_back(Vector3f(0, 10, 0));
			pointsToTest.push_back(Vector3f(0, -10, 0));
			pointsToTest.push_back(Vector3f(0, 0, 10));
			pointsToTest.push_back(Vector3f(0, 0, -10));

			DebugDraw debugDraw;

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
				if (EventSystem::ShouldClose())
				{
					window.Close();
				}

#ifdef ENLIVE_ENABLE_IMGUI
				ImGuiWrapper::BeginFrame(250);

				ImGuiToolManager::GetInstance().Update();

				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				const auto& selectedEntities = ImGuiEntityBrowser::GetInstance().GetSelectedEntities();
				for (const auto& enttEntity : selectedEntities)
				{
					Entity entity(world, enttEntity);
					if (entity && entity.Has<TransformComponent>())
					{
						ImGuizmo::Manipulate(
							camera.GetViewMatrix().GetData(),
							camera.GetProjectionMatrix().GetData(),
							ImGuizmo::TRANSLATE,
							ImGuizmo::WORLD,
							entity.Get<TransformComponent>().transform.GetMatrix().GetData()
						);
					}
				}

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

				spriteBTransform *= Matrix4f::RotationY(180.0f * dt.AsSeconds());

				// Camera movement
				if (Keyboard::IsAltHold())
				{
					Vector3f direction = camera.GetDirection();
					Vector3f mvtUnit = direction;
					mvtUnit.y = 0.0f;
					mvtUnit.Normalize();

					Vector3f movement;
					bool moved = false;
					if (Keyboard::IsHold(Keyboard::Key::W))
					{
						movement += 2.0f * mvtUnit * dt.AsSeconds();
						moved = true;
					}
					if (Keyboard::IsHold(Keyboard::Key::S))
					{
						movement -= 2.0f * mvtUnit * dt.AsSeconds();
						moved = true;
					}
					if (Keyboard::IsHold(Keyboard::Key::A))
					{
						movement -= 2.0f * mvtUnit.CrossProduct(Vector3f::UnitY()) * dt.AsSeconds();
						moved = true;
					}
					if (Keyboard::IsHold(Keyboard::Key::D))
					{
						movement += 2.0f * mvtUnit.CrossProduct(Vector3f::UnitY()) * dt.AsSeconds();
						moved = true;
					}
					if (moved)
					{
						camera.Move(movement);
					}

					bool rotated = false;

					F32 yaw = static_cast<F32>(Mouse::GetMouseMovement().x);
					F32 pitch = static_cast<F32>(Mouse::GetMouseMovement().y);
					if (!Math::Equals(yaw, 0.0f))
					{
						yaw *= 20.0f * dt.AsSeconds();
						direction = Matrix3f::RotationY(yaw).TransformDirection(direction);
						rotated = true;
					}
					if (!Math::Equals(pitch, 0.0f))
					{
						pitch *= 15.0f * dt.AsSeconds();
						direction = Matrix3f::RotationX(pitch).TransformDirection(direction);
						rotated = true;
					}

					if (rotated)
					{
						camera.SetDirection(direction);
					}
				}

				// Toggle debug stats
#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
				if (EventSystem::IsButtonActive(toggleGraphStats))
				{
					BgfxWrapper::ToggleDisplayStats();
				}
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

				debugDraw.DrawCross(Vector3f(-1.0f));
				debugDraw.DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);
				debugDraw.DrawFrustum(frustum, Colors::DarkBlue);
				for (const auto& point : pointsToTest)
				{
					debugDraw.DrawPoint(point, (frustum.Contains(point)) ? Colors::Green : Colors::Red);
				}
				debugDraw.DrawXZGrid(-10, 10, 0, 1, Colors::White);



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

					camera.Apply(mainViewId);

					world.Render();

					bgfx::setTransform(spriteBTransform.GetData());
					spriteB.Render(mainViewId);

					bgfx::setTransform(tilemapTransform.GetData());
					tilemap.Render(mainViewId);

					debugDraw.Render(mainViewId);

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
