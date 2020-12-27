#include <Enlivengine/Platform/PrimitiveTypes.hpp>
#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Window/Controller.hpp>
#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Window/Mouse.hpp>
#include <Enlivengine/Window/Keyboard.hpp>
#include <Enlivengine/Graphics/Framebuffer.hpp>
#include <Enlivengine/Resources/PathManager.hpp>
#include <Enlivengine/Core/Engine.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/TraceryGenerator.hpp>
#include <Enlivengine/Tools/ImGuiEditor.hpp>
#include <Enlivengine/Tools/ImGuiGame.hpp>

#include <Enlivengine/Core/Engine.hpp>
#include <Enlivengine/Meta/MetaSpecialization.hpp>

#include <Enlivengine/Core/ComponentManager.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/CameraComponent.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>

using namespace en;

class RenderSystem : public System
{
public:
	RenderSystem(World& world) : System(world) {}

	static const char* GetStaticName() { return "RenderSystem"; }
	const char* GetName() const override { return GetStaticName(); }

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
				const Matrix4f matrix = (entity.Has<TransformComponent>()) ? entity.Get<TransformComponent>().GetGlobalMatrix() : Matrix4f::Identity();
				if (entity.Has<SpriteComponent>())
				{
					bgfx::setTransform(matrix.GetData());
					entity.Get<SpriteComponent>().sprite.Render();
					render = true;
				}
				if (entity.Has<TilemapComponent>())
				{
					bgfx::setTransform(matrix.GetData());
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

class DebugSystem : public System
{
public:
	DebugSystem(World& world) : System(world) {}

	static const char* GetStaticName() { return "DebugSystem"; }
	const char* GetName() const override { return GetStaticName(); }

	void Render() override
	{
#ifdef ENLIVE_DEBUG
		mWorld.GetDebugDraw().DrawCross(Vector3f(-1.0f));
		mWorld.GetDebugDraw().DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);
		mWorld.GetDebugDraw().DrawSphere({ -1.0f, 0.5f, -3.0f }, 0.5f, Colors::Red);
		mWorld.GetDebugDraw().DrawGrid(Vector3f::Zero(), ENLIVE_DEFAULT_UP, -10, 10, 1, Colors::White);

		if (Camera* mainCamera = Camera::GetMainCamera())
		{
			mWorld.GetDebugDraw().DrawFrustum(mainCamera->CreateFrustum(), Colors::Blue);
			//mWorld.GetDebugDraw().DrawTransform(playerCamEntity.Get<TransformComponent>().GetGlobalMatrix());

#ifdef ENLIVE_TOOL
			if (ImGuiGame::IsMouseInView())
			{
				Vector3f mouseDir;
				const Vector3f mousePos = mainCamera->ScreenToWorldPoint(ImGuiGame::GetMouseScreenCoordinates(), &mouseDir);
				const Plane p(ENLIVE_DEFAULT_UP, 0.0f);
				const Ray r(mousePos, mouseDir);
				F32 t;
				if (r.Intersects(p, &t))
				{
					const Vector3f point = r.GetPoint(t);
					mWorld.GetDebugDraw().DrawLine(mousePos, point, Colors::Green);
					mWorld.GetDebugDraw().DrawSphere(point, 0.05f, Colors::Green);
				}
				else
				{
					mWorld.GetDebugDraw().DrawLine(mousePos, r.GetPoint(100.0f), Colors::Red);
				}
			}
#endif // ENLIVE_TOOL
		}
#endif // ENLIVE_DEBUG
	}
};

struct StupidShipComponent {};
ENLIVE_META_CLASS_BEGIN(StupidShipComponent)
ENLIVE_META_CLASS_END()

class StupidShipSystem : public System
{
public:
	StupidShipSystem(World& world) : System(world) {}

	static const char* GetStaticName() { return "StupidShipSystem"; }
	const char* GetName() const override { return GetStaticName(); }

	void Update(Time dt) override
	{
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<TransformComponent, StupidShipComponent>();
		for (auto entt : view)
		{
			Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				entity.Get<TransformComponent>().Rotate(Matrix3f::RotationY(180.0f * dt.AsSeconds()));
			}
		}
	}
};

struct PlayerComponent {};
ENLIVE_META_CLASS_BEGIN(PlayerComponent)
ENLIVE_META_CLASS_END()

class PlayerSystem : public System
{
public:
	PlayerSystem(World& world) : System(world) {}

	static const char* GetStaticName() { return "PlayerSystem"; }
	const char* GetName() const override { return GetStaticName(); }

	void Update(Time dt) override
	{
		if (EventSystem::IsButtonActive("action") || EventSystem::IsButtonActive("jactionP1"))
		{
			printf("Action!\n");
			Controller::Rumble(0, 0.25f, 100);
		}

		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<TransformComponent, PlayerComponent>();
		for (auto entt : view)
		{
			Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				const F32 forwardMvt = -Controller::GetAxis(0, 1);
				const F32 leftMvt = -Controller::GetAxis(0, 0);
				const F32 deltaYaw = Controller::GetAxis(0, 3);
				const F32 deltaPitch = Controller::GetAxis(0, 4);
				const F32 dtSeconds = dt.AsSeconds();

				TransformComponent& playerTransform = entity.Get<TransformComponent>();

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
	}
};


int main(int argc, char** argv)
{
	// Engine components
	ComponentManager::Register<NameComponent>();
	ComponentManager::Register<UIDComponent>();
	ComponentManager::Register<RenderableComponent>();
	ComponentManager::Register<SpriteComponent>();
	ComponentManager::Register<TilemapComponent>();
	ComponentManager::Register<CameraComponent>();
	ComponentManager::Register<TransformComponent>();

	// Own components
	ComponentManager::Register<StupidShipComponent>();
	ComponentManager::Register<PlayerComponent>();

	if (Engine::Init(argc, argv))
	{
		World world("TestWorld");
		world.CreateSystem<RenderSystem>();
		world.CreateSystem<StupidShipSystem>();
		world.CreateSystem<PlayerSystem>();
		world.CreateSystem<DebugSystem>();
		Engine::SetCurrentWorld(&world);

#ifdef ENLIVE_RELEASE
		world.Play();
#endif // ENLIVE_RELEASE

		{
			TexturePtr textureA = ResourceManager::GetInstance().Create<Texture>("textureA", TextureLoader::FromFile(PathManager::GetAssetsPath() + "fieldstone-rgba.dds"));
			if (!textureA.IsValid())
			{
				enAssert(false);
			}
			TexturePtr textureB = ResourceManager::GetInstance().Create<Texture>("textureB", TextureLoader::FromFile(PathManager::GetAssetsPath() + "ship_default.png"));
			if (!textureB.IsValid())
			{
				enAssert(false);
			}

			Tileset tileset;
			tileset.SetGridSize({ 2,2 });
			tileset.SetTileSize({ 256, 256 });
			tileset.SetTexture(textureA);

			Entity playerEntity = world.GetEntityManager().CreateEntity();
			{
				playerEntity.Add<NameComponent>().name = "Player";
				TransformComponent& playerTransform = playerEntity.Add<TransformComponent>();
				playerTransform.SetPosition(Vector3f(0.0f, 0.0f, 2.0f));
				playerTransform.SetRotation(Matrix3f::RotationY(0.0f));
				playerEntity.Add<PlayerComponent>();
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
				Camera::SetMainCamera(&playerCam);
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
				b1.Add<StupidShipComponent>();
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
		}

		Time dt;
		while (Engine::Update(dt))
		{
			// Update
			{
				world.Update(dt);

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
						world.Render();
						world.GetDebugDraw().Render();
					}
				}

				if (ImGuiEditor::IsViewVisible())
				{
					ImGuiEditor::GetCamera().Apply();
					world.Render();
					world.GetDebugDraw().Render();
				}
				world.GetDebugDraw().Clear();
#endif // ENLIVE_TOOL

#ifdef ENLIVE_RELEASE
				if (Camera* mainCamera = Camera::GetMainCamera())
				{
					mainCamera->Apply();
					world.Render();
#ifdef ENLIVE_DEBUG
					world.GetDebugDraw().Render();
					world.GetDebugDraw().Clear();
#endif // ENLIVE_DEBUG
				}
#endif // ENLIVE_RELEASE

				bgfx::frame();
			}
		}

		world.GetEntityManager().ClearEntities();

		ResourceManager::GetInstance().ReleaseAll();
	}
	Engine::Release();
	return 0;
}