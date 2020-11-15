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
#include <Enlivengine/Core/CameraComponent.hpp>

#include <Enlivengine/Meta/MetaSpecialization.hpp>

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
			if (entity)
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
				if (!render)
				{
					bgfx::setTransform(Matrix4f::Identity().GetData());
				}
			}
		}
		if (BgfxWrapper::GetCurrentView() == mToolView)
		{
			mWorld.GetDebugDraw().Render();
			mWorld.GetDebugDraw().Clear();
		}
	}

	void SetToolView(bgfx::ViewId viewId)
	{
		mToolView = viewId;
	}

private:
	bgfx::ViewId mToolView;
};

#ifdef ENLIVE_DEBUG
class ToolCameraSystem : public System
{
public:
	ToolCameraSystem(World& world) : System(world) 
	{
		const Vector2u framebufferSize = BgfxWrapper::GetFramebufferSize(mToolCamera.GetFramebuffer());
		mToolCamera.InitializePerspective(80.0f, F32(framebufferSize.x) / F32(framebufferSize.y), 0.1f, 100.0f);
		mToolCamera.InitializeView(Vector3f(0.0f, 0.8f, 0.0f), Matrix3f::Identity());
		mToolCamera.SetViewport(Rectf(0.0f, 0.0f, 1.0f, 1.0f));

		mCameraWindowResize.Connect(Window::GetFirstWindow()->OnResized, [&world, this](const Window*, U32 width, U32 height)
			{
				const Vector2u framebufferSize = BgfxWrapper::GetFramebufferSize(mToolCamera.GetFramebuffer());
				mToolCamera.InitializePerspective(80.0f, F32(width) / F32(height), 0.1f, 100.0f);
			});
	}

	void Update(Time dt) override
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
			FPSCamera(mToolCamera, dt.AsSeconds(), forward, left, deltaYaw, deltaPitch);
		}
		else
		{
			Mouse::SetRelativeMode(false);
		}
	}

	Camera& GetCamera()
	{
		return mToolCamera;
	}

	const Camera& GetCamera() const
	{
		return mToolCamera;
	}

private:
	Camera mToolCamera;
	enSlotType(Window, OnResized) mCameraWindowResize;
};
#endif // ENLIVE_DEBUG


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

			World world;
			RenderSystem* renderSystem = world.CreateSystem<RenderSystem>();
#ifdef ENLIVE_DEBUG
			ToolCameraSystem* toolCameraSystem = world.CreateSystem<ToolCameraSystem>();
			renderSystem->SetToolView(toolCameraSystem->GetCamera().GetViewID());
			bool useFreeCamera = true;
#endif // ENLIVE_DEBUG
			Universe::GetInstance().SetCurrentWorld(&world);

			Entity playerEntity = world.GetEntityManager().CreateEntity();
			{
				playerEntity.Add<NameComponent>().name = "Player";
				TransformComponent& playerTransform = playerEntity.Add<TransformComponent>();
				playerTransform.SetPosition(Vector3f(0.0f, 0.0f, 2.0f));
				playerTransform.SetRotation(Matrix3f::RotationY(0.0f));
			}

			Entity playerCamEntity = world.GetEntityManager().CreateEntity();
			{
				TransformComponent& playerCamTransform = playerCamEntity.Add<TransformComponent>();
				playerCamTransform.SetPosition(Vector3f(0.0f, 0.8f, 0.0f));
				playerCamTransform.AttachToParent(playerEntity); // Attach this entity to player entity
				CameraComponent& playerCam = playerCamEntity.Add<CameraComponent>();
				playerCam.InitializePerspective(80.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f);
				playerCam.SetViewport(Rectf(0.7f, 0.7f, 0.2f, 0.2f));
				playerCam.SetClearColor(Colors::LightBlue);
			}

			Entity a1 = world.GetEntityManager().CreateEntity();
			{
				a1.Add<NameComponent>().name = "A1";
				a1.Add<TransformComponent>().SetPosition(Vector3f(0.0f, 1.0f, 0.0f));
				a1.Add<RenderableComponent>();
				a1.Add<SpriteComponent>().sprite.SetTexture(textureA);
			}
			Entity a2 = world.GetEntityManager().CreateEntity();
			{
				a2.Add<NameComponent>().name = "A2";
				a2.Add<TransformComponent>().SetPosition(Vector3f(1.0f, 2.0f, 0.0f));
				a2.Add<RenderableComponent>();
				a2.Add<SpriteComponent>().sprite.SetTexture(textureA);
			}
			Entity b1 = world.GetEntityManager().CreateEntity();
			{
				b1.Add<NameComponent>().name = "B1";
				TransformComponent& b1Transform = b1.Add<TransformComponent>();
				b1Transform.SetPosition(Vector3f(2.0f, 2.0f, 0.0f));
				b1Transform.AttachChild(a2);
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

			enSlotType(Window, OnResized) cameraWindowResize;
			cameraWindowResize.Connect(window.OnResized, [&playerCamEntity](const Window*, U32 width, U32 height)
				{
					playerCamEntity.Get<CameraComponent>().InitializeOrthographic(-5.0f, -4.0f, 5.0f, 4.0f, 0.1f, 100.0f);
					//playerEntity.Get<CameraComponent>().InitializePerspective(80.0f, F32(width) / F32(height), 0.1f, 100.0f);
				});

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

			ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;
			const U32 gizmoTranslate = EventSystem::AddKeyButton("gizmoTranslate", Keyboard::Key::T, EventSystem::EventButton::ActionType::Pressed, static_cast<U32>(Keyboard::Modifier::Alt));
			const U32 gizmoRotate = EventSystem::AddKeyButton("gizmoRotate", Keyboard::Key::R, EventSystem::EventButton::ActionType::Pressed, static_cast<U32>(Keyboard::Modifier::Alt));
			const U32 gizmoScale = EventSystem::AddKeyButton("gizmoScale", Keyboard::Key::S, EventSystem::EventButton::ActionType::Pressed, static_cast<U32>(Keyboard::Modifier::Alt));

			Clock clock;

			while (!window.ShouldClose())
			{
				EventSystem::Update();

#ifdef ENLIVE_ENABLE_IMGUI
				ImGuiWrapper::BeginFrame(250, window.GetWidth(), window.GetHeight());

				ImGuiToolManager::GetInstance().Update();

#ifdef ENLIVE_DEBUG
				if (EventSystem::IsButtonActive(gizmoTranslate)) gizmoOperation = ImGuizmo::TRANSLATE;
				if (EventSystem::IsButtonActive(gizmoRotate)) gizmoOperation = ImGuizmo::ROTATE;
				if (EventSystem::IsButtonActive(gizmoScale)) gizmoOperation = ImGuizmo::SCALE;
				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				if (world.GetMainCamera() != nullptr)
				{
					const auto& selectedEntities = world.GetSelectedEntities();
					for (const auto& enttEntity : selectedEntities)
					{
						Entity entity(world, enttEntity);
						if (entity && entity.Has<TransformComponent>())
						{
							ImGuizmo::Manipulate(
								world.GetMainCamera()->GetViewMatrix().GetData(),
								world.GetMainCamera()->GetProjectionMatrix().GetData(),
								gizmoOperation,
								ImGuizmo::LOCAL,
								entity.Get<TransformComponent>().GetLocalMatrix().GetData()
							);
						}
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

				if (b1.IsValid() && b1.Has<TransformComponent>())
				{
					b1.Get<TransformComponent>().GetLocalMatrix().ApplyRotation(Matrix3f::RotationY(180.0f * dt.AsSeconds()));
				}

				if (Keyboard::IsPressed(Keyboard::Key::Escape))
				{
					window.Close();
				}
#ifdef ENLIVE_DEBUG
				if (Keyboard::IsPressed(Keyboard::Key::Space))
				{
					useFreeCamera = !useFreeCamera;
					if (useFreeCamera)
					{
						playerCamEntity.Get<CameraComponent>().SetViewport(Rectf(0.7f, 0.7f, 0.2f, 0.2f));
						toolCameraSystem->GetCamera().SetViewport(Rectf(0.0f, 0.0f, 1.0f, 1.0f));
					}
					else
					{
						toolCameraSystem->GetCamera().SetViewport(Rectf(0.7f, 0.7f, 0.2f, 0.2f));
						playerCamEntity.Get<CameraComponent>().SetViewport(Rectf(0.0f, 0.0f, 1.0f, 1.0f));
					}
				}
#endif // ENLIVE_DEBUG

#ifdef ENLIVE_ENABLE_GRAPHICS_DEBUG
				if (EventSystem::IsButtonActive(toggleGraphStats))
				{
					BgfxWrapper::ToggleDisplayStats();
				}
#endif // ENLIVE_ENABLE_GRAPHICS_DEBUG

				world.Update(dt);

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

				world.GetDebugDraw().DrawCross(Vector3f(-1.0f));
				world.GetDebugDraw().DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);
				world.GetDebugDraw().DrawSphere({ -1.0f, 0.5f, -3.0f }, 0.5f, Colors::Red);
#ifdef ENLIVE_DEBUG
				if (useFreeCamera)
				{
					world.GetDebugDraw().DrawFrustum(playerCamEntity.Get<CameraComponent>().CreateFrustum(), Colors::Blue);
					world.GetDebugDraw().DrawTransform(playerCamEntity.Get<TransformComponent>().GetGlobalMatrix());
				}
				else
				{
					world.GetDebugDraw().DrawFrustum(toolCameraSystem->GetCamera().CreateFrustum(), Colors::Green);
					world.GetDebugDraw().DrawTransform(Matrix4f::Transform(toolCameraSystem->GetCamera().GetPosition(), toolCameraSystem->GetCamera().GetRotation()));
				}
#endif // ENLIVE_DEBUG
				world.GetDebugDraw().DrawGrid(Vector3f::Zero(), ENLIVE_DEFAULT_UP, -10, 10, 1, Colors::White);

				Plane p(ENLIVE_DEFAULT_UP, 0.0f);
				Ray r(playerCamEntity.Get<TransformComponent>().GetGlobalPosition() + playerCamEntity.Get<CameraComponent>().GetPosition(), (playerEntity.Get<TransformComponent>().GetRotation().GetForward() * 3.0f - ENLIVE_DEFAULT_UP).Normalized());
				F32 t;
				if (r.Intersects(p, &t))
				{
					world.GetDebugDraw().DrawSphere(r.GetPoint(t), 0.1f);
				}

				// Render
				{

#ifdef ENLIVE_DEBUG
					// Render the other camera on the preview
					if (useFreeCamera)
					{
						playerCamEntity.Get<CameraComponent>().Apply();
						bgfx::setTransform(tilemapTransform.GetData());
						tilemap.Render(playerCamEntity.Get<CameraComponent>().GetViewID());
					}
					else
					{
						toolCameraSystem->GetCamera().Apply();
						bgfx::setTransform(tilemapTransform.GetData());
						tilemap.Render(toolCameraSystem->GetCamera().GetViewID());
					}
					world.Render();
#endif // ENLIVE_DEBUG

					// Main view
#ifdef ENLIVE_DEBUG
					// Debug texts
					{
						bgfx::dbgTextClear();

						/*
						const Vector2i dbgMousePos = Mouse::GetPositionCurrentWindow();
						const Vector2i dbgMouseDeltaPos = Mouse::GetMouseMovement();
						const I32 dbgMouseWheel = Mouse::GetWheel();
						bgfx::dbgTextPrintf(0, 0, 0x0f, "Mouse: (%d, %d) (%d, %d) (%d)", dbgMousePos.x, dbgMousePos.y, dbgMouseDeltaPos.x, dbgMouseDeltaPos.y, dbgMouseWheel);
						*/
						if (useFreeCamera)
						{
							const Vector3f position = toolCameraSystem->GetCamera().GetPosition();
							bgfx::dbgTextPrintf(0, 2, 0x0f, "Camera: (%f, %f, %f)", position.x, position.y, position.z);
						}
						else
						{
							const Vector3f position = playerCamEntity.Get<TransformComponent>().GetGlobalPosition() + playerCamEntity.Get<CameraComponent>().GetPosition();
							bgfx::dbgTextPrintf(0, 2, 0x0f, "Camera: (%f, %f, %f)", position.x, position.y, position.z);
						}

						{
							const Vector3f position = playerEntity.Get<TransformComponent>().GetGlobalPosition();
							bgfx::dbgTextPrintf(0, 3, 0x0f, "Position: (%f, %f, %f)", position.x, position.y, position.z);
						}
					}
#endif // ENLIVE_DEBUG

#ifdef ENLIVE_DEBUG
					if (useFreeCamera)
					{
						toolCameraSystem->GetCamera().Apply();
						bgfx::setTransform(tilemapTransform.GetData());
						tilemap.Render(toolCameraSystem->GetCamera().GetViewID());
					}
					else
					{
#endif // ENLIVE_DEBUG
						playerCamEntity.Get<CameraComponent>().Apply();
						bgfx::setTransform(tilemapTransform.GetData());
						tilemap.Render(playerCamEntity.Get<CameraComponent>().GetViewID());
#ifdef ENLIVE_DEBUG
					}
#endif // ENLIVE_DEBUG
					world.Render();

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