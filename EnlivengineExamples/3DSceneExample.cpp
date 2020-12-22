#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/SDL/SDLWrapper.hpp>
#include <Enlivengine/Window/Window.hpp>
#include <Enlivengine/Window/Controller.hpp>
#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Keyboard.hpp>
#include <Enlivengine/Graphics/BgfxWrapper.hpp>
#include <Enlivengine/Graphics/Framebuffer.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Tilemap.hpp>
#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/ComponentManager.hpp>
#include <Enlivengine/Core/TraceryGenerator.hpp>
#include <Enlivengine/Core/CameraComponent.hpp>
#include <Enlivengine/Meta/MetaSpecialization.hpp>
#include <Enlivengine/Tools/ImGuiToolManager.hpp>
#include <Enlivengine/Tools/ImGuiEditor.hpp>
#include <Enlivengine/Tools/ImGuiGame.hpp>

using namespace en;

void FPSCamera(Camera& camera, F32 dtSeconds, F32 forwardMvt, F32 leftMvt, F32 deltaYaw, F32 deltaPitch);

class RenderSystem : public System
{
public:
	RenderSystem(World& world) : System(world) {}

	void Render() override
	{
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<TransformComponent>();
		for (auto entt : view)
		{
			Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				bool render = false;
				if (entity.Has<TransformComponent>())
				{
					bgfx::setTransform(entity.Get<TransformComponent>().GetGlobalMatrix().GetData());
				}
				if (entity.Has<SpriteComponent>())
				{
					entity.Get<SpriteComponent>().sprite.Render();
					render = true;
				}
				if (entity.Has<TilemapComponent>())
				{
					entity.Get<TilemapComponent>().tilemap.Render();
					render = true;
				}
				if (!render)
				{
					bgfx::setTransform(Matrix4f::Identity().GetData());
				}
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
		window.Create("Test", 1600, 900);
		BgfxWrapper::Init(window);
		Sprite::InitializeSprites();
		Tilemap::InitializeTilemaps();
		DebugDraw::InitializeDebugDraws();

#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiToolManager::GetInstance().Initialize();
#ifdef ENLIVE_TOOL
		ImGuiToolManager::GetInstance().LoadFromFile(PathManager::GetAssetsPath() + "tools.json");
#endif // ENLIVE_TOOL
#endif // ENLIVE_ENABLE_IMGUI
		{
			ComponentManager::Register<NameComponent>();
			ComponentManager::Register<TransformComponent>();
			ComponentManager::Register<UIDComponent>();
			ComponentManager::Register<RenderableComponent>();
			ComponentManager::Register<SpriteComponent>();
			ComponentManager::Register<CameraComponent>();
			ComponentManager::Register<TilemapComponent>();

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

			Tileset tileset;
			tileset.SetGridSize({ 2,2 });
			tileset.SetTileSize({ 256, 256 });
			tileset.SetTexture(textureA);

#ifdef ENLIVE_TOOL
			ImGuiEditor::GetCamera().InitializePerspective(80.0f);
			ImGuiEditor::GetCamera().InitializeView(Vector3f(0.0f, 0.8f, 0.0f), Matrix3f::Identity());
#endif // ENLIVE_TOOL


			World world;
			world.CreateSystem<RenderSystem>();
			Universe::GetInstance().SetCurrentWorld(&world);

#ifdef ENLIVE_RELEASE
			world.Play();
#endif // ENLIVE_RELEASE

			Entity playerEntity = world.GetEntityManager().CreateEntity();
			{
				playerEntity.Add<NameComponent>().name = "Player";
				TransformComponent& playerTransform = playerEntity.Add<TransformComponent>();
				playerTransform.SetPosition(Vector3f(0.0f, 0.0f, 2.0f));
				playerTransform.SetRotation(Matrix3f::RotationY(0.0f));
			}

			Entity playerCamEntity = world.GetEntityManager().CreateEntity();
			{
				playerCamEntity.Add<NameComponent>().name = "PlayerCam";
				TransformComponent& playerCamTransform = playerCamEntity.Add<TransformComponent>();
				playerCamTransform.AttachToParent(playerEntity); // Attach this entity to player entity
				playerCamTransform.SetPosition(Vector3f(0.0f, 0.8f, 0.0f));
				CameraComponent& playerCam = playerCamEntity.Add<CameraComponent>();
				playerCam.InitializePerspective(80.0f);
#if defined(ENLIVE_TOOL)
				playerCam.SetFramebuffer(ImGuiGame::GetFramebuffer());
#elif defined(ENLIVE_RELEASE)
				playerCam.SetFramebuffer(&Framebuffer::GetDefaultFramebuffer());
#endif // ENLIVE_TOOL
				playerCam.SetClearColor(Colors::LightBlue);
			}

			Entity a1 = world.GetEntityManager().CreateEntity();
			{
				a1.Add<NameComponent>().name = "Wall1";
				a1.Add<TransformComponent>().SetPosition(Vector3f(0.0f, 1.0f, 0.0f));
				a1.Add<RenderableComponent>();
				a1.Add<SpriteComponent>().sprite.SetTexture(textureA);
			}
			Entity a2 = world.GetEntityManager().CreateEntity();
			{
				a2.Add<NameComponent>().name = "Wall2";
				a2.Add<TransformComponent>().SetPosition(Vector3f(1.0f, 1.0f, 0.0f));;
				a2.Add<RenderableComponent>();
				a2.Add<SpriteComponent>().sprite.SetTexture(textureA);
			}
			Entity b1 = world.GetEntityManager().CreateEntity();
			{
				b1.Add<NameComponent>().name = "Ship";
				TransformComponent& b1Transform = b1.Add<TransformComponent>();
				b1Transform.SetPosition(Vector3f(2.0f, 2.0f, 0.0f));
				b1.Add<RenderableComponent>();
				b1.Add<SpriteComponent>().sprite.SetTexture(textureB);
			}
			Entity c1 = world.GetEntityManager().CreateEntity();
			{
				c1.Add<NameComponent>().name = "Ground1";
				c1.Add<TransformComponent>().SetRotation(Matrix3f::RotationX(90.0f));
				c1.Add<RenderableComponent>();
				TilemapComponent& tilemapComponent = c1.Add<TilemapComponent>();
				tilemapComponent.tilemap.SetTileset(tileset);
				tilemapComponent.tilemap.SetSize({ 4,4 });
				tilemapComponent.tilemap.SetTile({ 1,1 }, 0);
				tilemapComponent.tilemap.SetTile({ 2,1 }, 1);
				tilemapComponent.tilemap.SetTile({ 2,2 }, 2);
				tilemapComponent.tilemap.SetTile({ 1,2 }, 3);
			}
			
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

				const Time dt = clock.Restart();

#ifdef ENLIVE_ENABLE_IMGUI
				ImGuiToolManager::GetInstance().Update(window, dt);
#endif // ENLIVE_ENABLE_IMGUI

				if (Keyboard::IsPressed(Keyboard::Key::Escape))
				{
					window.Close();
				}

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
				if (EventSystem::IsButtonActive(toggleGraphStats))
				{
					BgfxWrapper::ToggleDisplayStats();
				}
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

				world.Update(dt);

				if (world.IsPlaying())
				{
					if (EventSystem::IsButtonActive("action") || EventSystem::IsButtonActive("jactionP1"))
					{
						printf("Action!\n");
						if (!Controller::Rumble(0, 0.25f, 100))
						{
							printf("Can't rumble : %s\n", SDLWrapper::GetError());
						}
					}

					if (b1.IsValid() && b1.Has<TransformComponent>())
					{
						b1.Get<TransformComponent>().Rotate(Matrix3f::RotationY(180.0f * dt.AsSeconds()));
					}

					// Move player using controller
					{
						const F32 forwardMvt = -Controller::GetAxis(0, 1);
						const F32 leftMvt = -Controller::GetAxis(0, 0);
						const F32 deltaYaw = Controller::GetAxis(0, 3);
						const F32 deltaPitch = Controller::GetAxis(0, 4);
						const F32 dtSeconds = dt.AsSeconds();

						TransformComponent& playerTransform = playerEntity.Get<TransformComponent>();

						Vector3f direction = playerTransform.GetRotation().GetForward();

						// Movement
						if (forwardMvt != 0.0f || leftMvt != 0.0f)
						{
							Vector3f mvtUnit = direction;
							mvtUnit.y = 0.0f;
							mvtUnit.Normalize();

							Vector3f movement;
							movement += 3.0f * forwardMvt * mvtUnit * dtSeconds;
							movement -= 3.0f * leftMvt * mvtUnit.CrossProduct(ENLIVE_DEFAULT_UP) * dtSeconds;
							playerTransform.Move(movement);
						}

						// Rotation
						if (deltaYaw != 0.0f || deltaPitch != 0.0f)
						{
							if (!Math::Equals(deltaYaw, 0.0f))
							{
								playerTransform.SetRotation(playerTransform.GetRotation() * Matrix3f::RotationY(100.0f * dtSeconds * deltaYaw));
							}
							if (!Math::Equals(deltaPitch, 0.0f))
							{
								//playerTransform.Rotate(Quaternionf(100.0f * dtSeconds * deltaPitch, direction.CrossProduct(ENLIVE_DEFAULT_UP)));
							}
						}
					}
				}

#ifdef ENLIVE_DEBUG
				{
					world.GetDebugDraw().DrawCross(Vector3f(-1.0f));
					world.GetDebugDraw().DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);
					world.GetDebugDraw().DrawSphere({ -1.0f, 0.5f, -3.0f }, 0.5f, Colors::Red);
					world.GetDebugDraw().DrawFrustum(playerCamEntity.Get<CameraComponent>().CreateFrustum(), Colors::Blue);
					world.GetDebugDraw().DrawTransform(playerCamEntity.Get<TransformComponent>().GetGlobalMatrix());
					world.GetDebugDraw().DrawGrid(Vector3f::Zero(), ENLIVE_DEFAULT_UP, -10, 10, 1, Colors::White);

					if (ImGuiGame::IsMouseInView())
					{
						Vector3f mouseDir;
						const Vector3f mousePos = playerCamEntity.Get<CameraComponent>().ScreenToWorldPoint(ImGuiGame::GetMouseScreenCoordinates(), &mouseDir);
						const Plane p(ENLIVE_DEFAULT_UP, 0.0f);
						const Ray r(mousePos, mouseDir);
						F32 t;
						if (r.Intersects(p, &t))
						{
							const Vector3f point = r.GetPoint(t);
							world.GetDebugDraw().DrawLine(mousePos, point, Colors::Green);
							world.GetDebugDraw().DrawSphere(point, 0.05f, Colors::Green);
						}
						else
						{
							world.GetDebugDraw().DrawLine(mousePos, r.GetPoint(100.0f), Colors::Red);
						}
					}
				}
#endif // ENLIVE_DEBUG

				// Render
				{
#ifdef ENLIVE_TOOL
					if (ImGuiGame::IsViewVisible())
					{
						playerCamEntity.Get<CameraComponent>().Apply();
						world.Render();
					}

					if (ImGuiEditor::IsViewVisible())
					{
						if (Keyboard::IsAltHold())
						{
							Mouse::SetRelativeMode(true);

							F32 forward = 0.0f;
							F32 left = 0.0;
							if (Keyboard::IsHold(Keyboard::Key::W)) forward += 1.0f;
							if (Keyboard::IsHold(Keyboard::Key::S)) forward -= 1.0f;
							if (Keyboard::IsHold(Keyboard::Key::A)) left += 1.0f;
							if (Keyboard::IsHold(Keyboard::Key::D)) left -= 1.0f;
							const F32 deltaYaw = Mouse::GetMouseMovement().x * 0.25f;
							const F32 deltaPitch = Mouse::GetMouseMovement().y * 0.15f;
							FPSCamera(ImGuiEditor::GetCamera(), dt.AsSeconds(), forward, left, deltaYaw, deltaPitch);
						}
						else
						{
							Mouse::SetRelativeMode(false);
						}

						ImGuiEditor::GetCamera().Apply();
						world.Render();
						world.GetDebugDraw().Render();
						world.GetDebugDraw().Clear();
					}
#endif // ENLIVE_TOOL

#ifdef ENLIVE_RELEASE
					playerCamEntity.Get<CameraComponent>().Apply();
					world.Render();
#ifdef ENLIVE_DEBUG
					world.GetDebugDraw().Render();
					world.GetDebugDraw().Clear();
#endif // ENLIVE_DEBUG
#endif // ENLIVE_RELEASE

					bgfx::frame();
				}
			}

			world.GetEntityManager().ClearEntities();

			textureA.ReleaseFromManager();
			textureB.ReleaseFromManager();
		}
#ifdef ENLIVE_ENABLE_IMGUI
#ifdef ENLIVE_TOOL
		ImGuiToolManager::GetInstance().SaveToFile(PathManager::GetAssetsPath() + "tools.json");
#endif // ENLIVE_TOOL
		ImGuiToolManager::GetInstance().Release();
#endif // ENLIVE_ENABLE_IMGUI

		DebugDraw::ReleaseDebugDraws();
		Tilemap::ReleaseTilemaps();
		Sprite::ReleaseSprites();
		BgfxWrapper::Release();
	}
	SDLWrapper::Release();
	return 0;
}

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
			camera.Rotate(Matrix3f::RotationY(100.0f * dtSeconds * deltaYaw));
		}
		if (!Math::Equals(deltaPitch, 0.0f))
		{
			//camera.Rotate(Quaternionf(100.0f * dtSeconds * deltaPitch, direction.CrossProduct(ENLIVE_DEFAULT_UP)));
		}
	}
}