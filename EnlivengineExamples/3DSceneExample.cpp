#include <Enlivengine/Engine.hpp>

#include <Enlivengine/Utils/Meta.hpp>
#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Window/Controller.hpp>
#include <Enlivengine/Graphics/Camera.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Tools/ImGuiGame.hpp>

using namespace en;

class RenderSystem : public RenderSystemBase
{
public:
	const char* GetName() const override;
	bool Serialize(Serializer& serializer, const char* name) override;
	bool Edit(ObjectEditor& objectEditor, const char* name) override;

	void Render() override
	{
		auto& entityManager = mWorld->GetEntityManager();
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
ENLIVE_META_CLASS_BEGIN(RenderSystem, en::Type_ClassSerialization, en::Type_ClassEditor)
ENLIVE_META_CLASS_END()
const char* RenderSystem::GetName() const 
{ 
	return TypeInfo<RenderSystem>::GetName(); 
}
bool RenderSystem::Serialize(Serializer& serializer, const char* name)
{
	return GenericSerialization(serializer, name, *this);
}
bool RenderSystem::Edit(ObjectEditor& objectEditor, const char* name)
{
	return GenericEdit(objectEditor, name, *this);
}

class DebugSystem : public System
{
public:
	const char* GetName() const override;
	bool Serialize(Serializer& serializer, const char* name) override;
	bool Edit(ObjectEditor& objectEditor, const char* name) override;

	void Update(Time dt) override
	{
#ifdef ENLIVE_DEBUG
		mWorld->GetDebugDraw().DrawCross(Vector3f(-1.0f));
		mWorld->GetDebugDraw().DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);
		mWorld->GetDebugDraw().DrawSphere({ -1.0f, 0.5f, -3.0f }, 0.5f, Colors::Red);
		mWorld->GetDebugDraw().DrawGrid(Vector3f::Zero(), ENLIVE_DEFAULT_UP, -10, 10, 1, Colors::White);

		if (Camera* mainCamera = Camera::GetMainCamera())
		{
			mWorld->GetDebugDraw().DrawFrustum(mainCamera->CreateFrustum(), Colors::Blue);
			//mWorld->GetDebugDraw().DrawTransform(playerCamEntity.Get<TransformComponent>().GetGlobalMatrix());

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
					mWorld->GetDebugDraw().DrawLine(mousePos, point, Colors::Green);
					mWorld->GetDebugDraw().DrawSphere(point, 0.05f, Colors::Green);
				}
				else
				{
					mWorld->GetDebugDraw().DrawLine(mousePos, r.GetPoint(100.0f), Colors::Red);
				}
			}
#endif // ENLIVE_TOOL
		}
#endif // ENLIVE_DEBUG
	}
};
ENLIVE_META_CLASS_BEGIN(DebugSystem, en::Type_ClassSerialization, en::Type_ClassEditor)
ENLIVE_META_CLASS_END()
const char* DebugSystem::GetName() const
{
	return TypeInfo<DebugSystem>::GetName();
}
bool DebugSystem::Serialize(Serializer& serializer, const char* name)
{
	return GenericSerialization(serializer, name, *this);
}
bool DebugSystem::Edit(ObjectEditor& objectEditor, const char* name)
{
	return GenericEdit(objectEditor, name, *this);
}

struct StupidShipComponent {};
ENLIVE_META_CLASS_BEGIN(StupidShipComponent, en::Type_ClassSerialization, en::Type_ClassEditor)
ENLIVE_META_CLASS_END()

class StupidShipSystem : public System
{
public:
	const char* GetName() const override;
	bool Serialize(Serializer& serializer, const char* name) override;
	bool Edit(ObjectEditor& objectEditor, const char* name) override;

	void Update(Time dt) override
	{
		auto& entityManager = mWorld->GetEntityManager();
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
ENLIVE_META_CLASS_BEGIN(StupidShipSystem, en::Type_ClassSerialization, en::Type_ClassEditor)
ENLIVE_META_CLASS_END()
const char* StupidShipSystem::GetName() const
{
	return TypeInfo<StupidShipSystem>::GetName();
}
bool StupidShipSystem::Serialize(Serializer& serializer, const char* name)
{
	return GenericSerialization(serializer, name, *this);
}
bool StupidShipSystem::Edit(ObjectEditor& objectEditor, const char* name)
{
	return GenericEdit(objectEditor, name, *this);
}

struct PlayerComponent {};
ENLIVE_META_CLASS_BEGIN(PlayerComponent, en::Type_ClassSerialization, en::Type_ClassEditor)
ENLIVE_META_CLASS_END()

class PlayerSystem : public System
{
public:
	const char* GetName() const override;
	bool Serialize(Serializer& serializer, const char* name) override;
	bool Edit(ObjectEditor& objectEditor, const char* name) override;

	void Update(Time dt) override
	{
		if (EventSystem::IsButtonActive("action") || EventSystem::IsButtonActive("jactionP1"))
		{
			printf("Action!\n");
			Controller::Rumble(0, 0.25f, 100);
		}

		auto& entityManager = mWorld->GetEntityManager();
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
ENLIVE_META_CLASS_BEGIN(PlayerSystem, en::Type_ClassSerialization, en::Type_ClassEditor)
ENLIVE_META_CLASS_END()
const char* PlayerSystem::GetName() const
{
	return TypeInfo<PlayerSystem>::GetName();
}
bool PlayerSystem::Serialize(Serializer& serializer, const char* name)
{
	return GenericSerialization(serializer, name, *this);
}
bool PlayerSystem::Edit(ObjectEditor& objectEditor, const char* name)
{
	return GenericEdit(objectEditor, name, *this);
}

int main(int argc, char** argv)
{
	Engine::RegisterComponent<StupidShipComponent>();
	Engine::RegisterComponent<PlayerComponent>();

	Engine::RegisterSystem<RenderSystem>();
	Engine::RegisterSystem<DebugSystem>();
	Engine::RegisterSystem<StupidShipSystem>();
	Engine::RegisterSystem<PlayerSystem>();

	return Engine::Main(argc, argv);
}