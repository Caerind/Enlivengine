#include <Enlivengine/Engine.hpp>

#include <Enlivengine/Window/EventSystem.hpp>
#include <Enlivengine/Window/Controller.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/TransformComponent.hpp>
#include <Enlivengine/Core/ViewComponent.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Graphics/View.hpp>
#include <Enlivengine/Tools/ImGuiEditor.hpp>

#include <Enlivengine/Audio/AudioManager.hpp>
#include <Enlivengine/Audio/AudioSourceWav.hpp>
#include <Enlivengine/Audio/AudioSourceSpeech.hpp>

using namespace en;

class RenderSystem : public RenderSystemBase
{
public:
	const char* GetName() const override;
	bool Serialize(Serializer& serializer, const char* name) override;
	bool Edit(ObjectEditor& objectEditor, const char* name) override;

	void Render(RenderTarget& renderTarget) override
	{
#ifdef ENLIVE_DEBUG
		// TODO
		//mWorld->GetDebugDraw().DrawCross(Vector3f(0.0f));
		//mWorld->GetDebugDraw().DrawGrid(Vector3f::Zero(), ENLIVE_DEFAULT_UP, -16, 16, 1, Colors::White);
#endif // ENLIVE_DEBUG

		const View& currentView = GetCurrentView();

		if (renderTarget.IsValid())
		{
			const Vector2u& renderTargetSize = renderTarget.GetSize();
			const F32 aspectRatio = static_cast<F32>(renderTargetSize.x) / static_cast<F32>(renderTargetSize.y);

			// TODO : Viewport
			//const Vector2f vpMin = currentView.GetViewport().GetMin();
			//const Vector2f vpSize = currentView.GetViewport().GetSize();

            // TODO : Aspect
			/*
			if (currentView.GetAspect() != aspectRatio)
			{
				currentView.SetAspect(aspectRatio);
			}
			*/

			renderTarget.Clear();
			// TODO : Set view from Camera
			//bgfx::setViewTransform(mainViewID, camera->GetViewMatrix().GetData(), camera->GetProjectionMatrix().GetData());
			//bgfx::setViewRect(mainViewID, static_cast<U16>(vpMin.x * renderTargetSize.x), static_cast<U16>(vpMin.y * renderTargetSize.y), static_cast<U16>(vpSize.x * renderTargetSize.x), static_cast<U16>(vpSize.y * renderTargetSize.y));

			RenderWorld();

#ifdef ENLIVE_DEBUG
#ifdef ENLIVE_TOOL
			if (ImGuiEditor::IsShowingDebug())
#else
			if (mWorld->IsDebugRendering())
#endif // ENLIVE_TOOL
			{
				mWorld->GetDebugDraw().Render(renderTarget);
			}
#endif // ENLIVE_DEBUG
		}
	}

	void RenderWorld()
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
					/*
					Sprite& sprite = entity.Get<SpriteComponent>().sprite;
					if (sprite.CanRender())
					{
						// TODO : Transform
						//bgfx::setTransform(matrix.GetData());
						sprite.Render();
						render = true;
					}
					*/
				}
				if (entity.Has<TilemapComponent>())
				{
					/*
					Tilemap& tilemap = entity.Get<TilemapComponent>().tilemap;
					if (tilemap.CanRender())
                    {
                        // TODO : Transform
						//bgfx::setTransform(matrix.GetData());
						entity.Get<TilemapComponent>().tilemap.Render();
						render = true;
					}
					*/
				}
				if (!render)
                {
                    // TODO : Transform
					//bgfx::setTransform(Matrix4f::Identity().GetData());
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
	DebugSystem() : System()
	{
#ifdef ENLIVE_TOOL
		mUpdateOnTool = true;
#endif // ENLIVE_TOOL
	}

	const char* GetName() const override;
	bool Serialize(Serializer& serializer, const char* name) override;
	bool Edit(ObjectEditor& objectEditor, const char* name) override;

	void Update() override
	{
#ifdef ENLIVE_DEBUG
		// TODO
		//mWorld->GetDebugDraw().DrawBox({ 1.0f, 0.5f, 1.0f }, { 2.0f, 1.5f, 2.0f }, Colors::Red);
		//mWorld->GetDebugDraw().DrawSphere({ -1.0f, 0.5f, -3.0f }, 0.5f, Colors::Red);

		// TODO
		/*
		auto& entityManager = mWorld->GetEntityManager();
		auto view = entityManager.View<en::CameraComponent>();
		for (auto entt : view)
		{
			Entity entity(entityManager, entt);
			if (entity.IsValid() && mWorld->IsSelected(entity))
			{
				auto& cam = entity.Get<en::CameraComponent>();
				mWorld->GetDebugDraw().DrawFrustum(cam.CreateFrustum());
			}
		}
		*/
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

	void Update() override
	{
		auto& entityManager = mWorld->GetEntityManager();
		auto view = entityManager.View<TransformComponent, StupidShipComponent>();
		for (auto entt : view)
		{
			Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				entity.Get<TransformComponent>().Rotate(Matrix3f::RotationY(180.0f * Time::GetDeltaTime().AsSeconds()));
			}
		}

		static bool played = false;
		if (!played)
		{
			speech.SetText("1, 2, 3");
			AudioManager::GetInstance().PlaySource(speech);
			AudioManager::GetInstance().PlaySource(ResourceManager::GetInstance().Get<AudioSourceWav>("theme").Get());
			played = true;
		}

	}

	AudioSourceSpeech speech;
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

	void Update() override
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
				const F32 dtSeconds = Time::GetDeltaTime().AsSeconds();

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
