#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Graphics/View.hpp>
#include <Enlivengine/System/ClassManager.hpp>
#include <Enlivengine/Core/ComponentManager.hpp>
#include <Enlivengine/Core/Universe.hpp>
#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/PhysicSystem.hpp>
#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/PhysicComponent.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

//////////////////////////////////
// Components
//////////////////////////////////

struct PlayerComponent
{
	en::U32 id; // Just to not be empty
};

struct MarkedForDestructionComponent
{
	en::U32 id; // Just to not be empty
};

struct CanJumpComponent
{
	bool CanJump() const
	{
		return numContacts > 0;
	}

	en::I32 numContacts{ 0 };

	en::PhysicSystem::BeginContactSlotType beginContactSlot;
	en::PhysicSystem::EndContactSlotType endContactSlot;
};

template <>
struct CustomComponentInitialization<CanJumpComponent>
{
	static constexpr bool value = true;
	static bool Initialize(const en::Entity& entity, CanJumpComponent& component)
	{
		if (entity.IsValid() && entity.Has<en::PhysicComponent>())
		{
			const en::PhysicComponent& physComp = entity.Get<en::PhysicComponent>();

			en::World& world = const_cast<en::World&>(entity.GetWorld());
			if (world.HasPhysicSystem())
			{
				en::PhysicSystem* physicSystem = world.GetPhysicSystem();

				physicSystem->AddBeginContactSlot(component.beginContactSlot, physComp, [&component](b2Contact*, b2Fixture* fixture, b2Fixture* otherFixture)
				{
					if (fixture != nullptr && otherFixture != nullptr)
					{
						if (fixture->IsSensor() && fixture->GetBody() != otherFixture->GetBody())
						{
							component.numContacts++;
						}
					}
				});
				physicSystem->AddEndContactSlot(component.endContactSlot, physComp, [&component](b2Contact*, b2Fixture* fixture, b2Fixture* otherFixture)
				{
					if (fixture != nullptr && otherFixture != nullptr)
					{
						if (fixture->IsSensor() && fixture->GetBody() != otherFixture->GetBody())
						{
							component.numContacts--;
						}
					}
				});

				return true;
			}
		}
		return false;
	}
};

struct ProjectileComponent
{
	en::Time lifetime{ en::Time::Seconds(5.0f) };
	en::PhysicSystem::BeginContactSlotType beginContactSlot;
};

template <>
struct CustomComponentInitialization<ProjectileComponent>
{
	static constexpr bool value = true;
	static bool Initialize(const en::Entity& entity, ProjectileComponent& component)
	{
		if (entity.IsValid() && entity.Has<en::PhysicComponent>())
		{
			const en::PhysicComponent& physComp = entity.Get<en::PhysicComponent>();

			en::World& world = const_cast<en::World&>(entity.GetWorld());
			if (world.HasPhysicSystem())
			{
				en::PhysicSystem* physicSystem = world.GetPhysicSystem();

				physicSystem->AddBeginContactSlot(component.beginContactSlot, physComp, [&component](b2Contact* contact, b2Fixture* fixture, b2Fixture* otherFixture)
				{
					if (fixture != nullptr && otherFixture != nullptr && fixture->GetBody() != otherFixture->GetBody() && contact != nullptr && contact->IsTouching())
					{
						if (en::PhysicComponent* physComp = static_cast<en::PhysicComponent*>(fixture->GetBody()->GetUserData()))
						{
							en::Entity entity = physComp->GetEntity(); // TODO : This is buggy af
							if (entity.IsValid() && !entity.Has<MarkedForDestructionComponent>())
							{
								entity.Add<MarkedForDestructionComponent>();
							}
						}
					}
				});

				return true;
			}
		}
		return false;
	}
};


//////////////////////////////////
// Systems
//////////////////////////////////

// Transform -> Physic
class BeforePhysicSystem : public en::System
{
public:
	BeforePhysicSystem(en::World& world) : en::System(world) {}

	void Update(en::Time dt) override
	{
		ENLIVE_UNUSED(dt);
		ENLIVE_PROFILE_FUNCTION();
		if (const auto* physicSystem = mWorld.GetPhysicSystem())
		{
			const auto pixelsPerMeter = physicSystem->GetPixelsPerMeter();
			auto& entityManager = mWorld.GetEntityManager();
			auto view = entityManager.View<en::TransformComponent, en::PhysicComponent>();
			for (auto entt : view)
			{
				en::Entity entity(entityManager, entt);
				if (entity.IsValid())
				{
					auto& physicComponent = entity.Get<en::PhysicComponent>();
					if (auto* body = physicComponent.GetBody())
					{
						const auto& transformComponent = entity.Get<en::TransformComponent>();
						const auto position = transformComponent.transform.GetPosition2D();
						body->SetTransform(b2Vec2(position.x / pixelsPerMeter, position.y / pixelsPerMeter), en::Math::DegToRad(transformComponent.transform.GetRotation2D()));
					}
				}
			}
		}
	}
};

// Physic -> Transform
class AfterPhysicSystem : public en::System
{
public:
	AfterPhysicSystem(en::World& world) : en::System(world) {}
	
	void Update(en::Time dt) override
	{
		ENLIVE_UNUSED(dt);
		ENLIVE_PROFILE_FUNCTION();
		if (const auto* physicSystem = mWorld.GetPhysicSystem())
		{
			const auto pixelsPerMeter = physicSystem->GetPixelsPerMeter();
			auto& entityManager = mWorld.GetEntityManager();
			auto view = entityManager.View<en::TransformComponent, en::PhysicComponent>();
			for (auto entt : view)
			{
				en::Entity entity(entityManager, entt);
				if (entity.IsValid())
				{
					const auto& physicComponent = entity.Get<en::PhysicComponent>();
					if (physicComponent.GetBodyType() != en::PhysicBodyType::Static)
					{
						if (const auto* body = physicComponent.GetBody())
						{
							const auto& position = body->GetPosition();
							auto& transformComponent = entity.Get<en::TransformComponent>();
							transformComponent.transform.SetPosition(position.x * pixelsPerMeter, position.y * pixelsPerMeter);
							transformComponent.transform.SetRotation2D(en::Math::RadToDeg(body->GetAngle()));
						}
					}
				}
			}
		}
	}
};

class PlayerMovementSystem : public en::System
{
public:
	PlayerMovementSystem(en::World& world) : en::System(world) {}

	void Update(en::Time dt) override
	{
		static constexpr en::F32 velocityLimit = 10.0f;
		static constexpr en::F32 inputValue = 0.1f;
		static constexpr en::F32 brakingFactor = 0.98f;
		static constexpr en::F32 jumpStrength = 10.0f;
		auto& actionSystem = en::Application::GetInstance().GetActionSystem();
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<PlayerComponent, CanJumpComponent, en::PhysicComponent>();
		for (auto entt : view)
		{
			en::Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				auto& physComponent = entity.Get<en::PhysicComponent>();
				if (auto* body = physComponent.GetBody())
				{
					const en::F32 mass = physComponent.GetMass();

					const en::F32 velocity = physComponent.GetLinearVelocity().x;
					bool input = false;
					en::F32 inputVelocity = 0.0f;
					if (actionSystem.IsInputActive("moveRight"))
					{
						input = true;
						inputVelocity += inputValue;
					}
					if (actionSystem.IsInputActive("moveLeft"))
					{
						input = true;
						inputVelocity -= inputValue;
					}
					en::F32 desiredVelocity = 0.0f;
					if (input)
					{
						desiredVelocity = en::Math::Clamp(velocity + inputVelocity, -velocityLimit, +velocityLimit);
					}
					else
					{
						desiredVelocity = velocity * brakingFactor;
					}
					const en::F32 velocityChange = desiredVelocity - velocity;
					if (!en::Math::Equals(velocityChange, 0.0f, 0.001f))
					{
						const en::F32 impulse = mass * velocityChange;
						body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0.0f), true);
					}

					const auto& jumpComp = entity.Get<CanJumpComponent>();
					if (jumpComp.CanJump() && actionSystem.IsInputActive("jump"))
					{
						const en::F32 impulse = mass * jumpStrength;
						body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -impulse), true);
					}
				}
			}
		}
	}
};

class PlayerShootSystem : public en::System
{
public:
	PlayerShootSystem(en::World& world) : en::System(world), accumulator(en::Time::Seconds(-2.0f)) {}

	void Update(en::Time dt) override
	{
		accumulator += dt;
		if (accumulator >= kCooldownTime)
		{
			accumulator = en::Time::Zero();
			const auto mousePos = en::Application::GetInstance().GetWindow().getCursorPosition();
			auto& entityManager = mWorld.GetEntityManager();
			auto view = entityManager.View<PlayerComponent, en::TransformComponent>();
			for (auto entt : view)
			{
				en::Entity entity(entityManager, entt);
				if (entity.IsValid())
				{
					const auto pos = entity.GetPosition2D();
					auto delta = mousePos - pos;
					auto angle = delta.GetPolarAngle();
					auto polar = en::Vector2f::Polar(angle);
					const en::F32 bulletStartOffset = 1.5f * mWorld.GetPhysicSystem()->GetPixelsPerMeter();
					const en::F32 bulletLinearSpeed = 40.0f;

					auto bulletEntity = mWorld.GetEntityManager().CreateEntity();
					bulletEntity.Add<en::NameComponent>("Bullet");
					bulletEntity.Add<en::RenderableComponent>();
					auto& bulletTransform = bulletEntity.Add<en::TransformComponent>().transform;
					bulletTransform.SetPosition(pos + polar * bulletStartOffset);
					bulletTransform.SetRotation2D(angle);
					auto& bulletPhys = bulletEntity.Add<en::PhysicComponent>();
					bulletPhys.SetBodyType(en::PhysicBodyType::Dynamic);
					bulletPhys.SetBullet(true);
					bulletPhys.SetLinearVelocity(polar * bulletLinearSpeed);
					// TODO : Improve fixture via code
					b2PolygonShape bulletShape;
					bulletShape.SetAsBox(0.25f, 0.125f);
					b2FixtureDef bulletFixture;
					bulletFixture.shape = &bulletShape;
					bulletPhys.GetBody()->CreateFixture(&bulletFixture);
					bulletEntity.Add<ProjectileComponent>();
				}
			}
		}
	}

	en::Time accumulator;
	static constexpr en::Time kCooldownTime = en::Time::Seconds(0.3f);
};

class ProjectileLifetimeSystem : public en::System
{
public:
	ProjectileLifetimeSystem(en::World& world) : en::System(world) {}

	void Update(en::Time dt) override
	{
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<ProjectileComponent>();
		for (auto entt : view)
		{
			en::Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				auto& proj = entity.Get<ProjectileComponent>();
				proj.lifetime -= dt;
				if (proj.lifetime <= 0.0f && !entity.Has<MarkedForDestructionComponent>())
				{
					entity.Add<MarkedForDestructionComponent>();
				}
			}
		}
	}
};

class DestructionSystem : public en::System
{
public:
	DestructionSystem(en::World& world) : en::System(world) {}

	void Update(en::Time dt) override
	{
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<MarkedForDestructionComponent>();
		for (auto entt : view)
		{
			en::Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				entity.Destroy();
			}
		}
	}
};

class TransformRenderSystem : public en::System
{
public:
	TransformRenderSystem(en::World& world) : en::System(world) 
	{
		static constexpr en::F32 size = 10.0f;
		tX.setSize({ size, 1.0f });
		tX.setFillColor(sf::Color::Green);
		tX.setOrigin({ 0.0f, 0.5f });
		tY.setSize({ 1.0f, size });
		tY.setFillColor(sf::Color::Red);
		tY.setOrigin({ 0.5f, 0.0f });

		static constexpr en::F32 radius = 5.0f;
		dot.setRadius(radius);
		dot.setOrigin(radius, radius);
		dot.setFillColor(sf::Color::White);
	}

	void Render(sf::RenderTarget& target) override
	{
		auto& entityManager = mWorld.GetEntityManager();
		auto view = entityManager.View<en::RenderableComponent>();
		for (auto entt : view)
		{
			en::Entity entity(entityManager, entt);
			if (entity.IsValid())
			{
				sf::RenderStates states;
				if (entity.Has<en::TransformComponent>())
				{
					states.transform = en::toSF(entity.Get<en::TransformComponent>().transform.GetMatrix());
				}
				target.draw(tX, states);
				target.draw(tY, states);
				target.draw(dot, states);
			}
		}
	}

	sf::RectangleShape tX;
	sf::RectangleShape tY;
	sf::CircleShape dot;
};


//////////////////////////////////
// State
//////////////////////////////////

class MyState : public en::State
{
public:
	MyState(en::StateManager& manager) : en::State(manager)
	{
		// World
		mWorld.CreateSystem<PlayerMovementSystem>();
		mWorld.CreateSystem<BeforePhysicSystem>();
		mWorld.CreateSystem<en::PhysicSystem>();
		mWorld.CreateSystem<AfterPhysicSystem>();
		mWorld.CreateSystem<PlayerShootSystem>();
		mWorld.CreateSystem<ProjectileLifetimeSystem>();
		mWorld.CreateSystem<DestructionSystem>();
		mWorld.CreateSystem<TransformRenderSystem>();

		en::Universe::GetInstance().SetCurrentWorld(&mWorld);
		mWorld.GetFreeCamView() = getApplication().GetWindow().getMainView();
		mWorld.GetGameView() = getApplication().GetWindow().getMainView();
		auto* physSystem = mWorld.GetPhysicSystem();
		physSystem->SetPixelsPerMeter(32.0f);
		physSystem->SetGravity({ 0.0f, 9.8f });
		physSystem->SetDebugRendering(true);

		// Ground
		auto groundEntity = mWorld.GetEntityManager().CreateEntity();
		groundEntity.Add<en::NameComponent>("Ground");
		groundEntity.Add<en::RenderableComponent>();
		auto& groundTransform = groundEntity.Add<en::TransformComponent>();
		groundTransform.transform.SetPosition(512.0f, 736.0f);
		auto& groundPhys = groundEntity.Add<en::PhysicComponent>();
		groundPhys.SetBodyType(en::PhysicBodyType::Static);
		// TODO : Improve fixture via code
		b2PolygonShape groundShape;
		groundShape.SetAsBox(16.0f, 1.0f);
		b2FixtureDef groundFixture;
		groundFixture.shape = &groundShape;
		groundPhys.GetBody()->CreateFixture(&groundFixture);

		// Player
		auto playerEntity = mWorld.GetEntityManager().CreateEntity();
		playerEntity.Add<en::NameComponent>("Player");
		playerEntity.Add<en::RenderableComponent>();
		playerEntity.Add<PlayerComponent>(); // Component to identify the player
		auto& playerTransform = playerEntity.Add<en::TransformComponent>();
		playerTransform.transform.SetPosition(512.0f, 64.0f);
		auto& playerPhys = playerEntity.Add<en::PhysicComponent>();
		playerPhys.SetBodyType(en::PhysicBodyType::Dynamic);
		playerPhys.SetFixedRotation(true);
		playerPhys.SetLinearDamping(0.1f);
		// TODO : Improve fixture via code
		b2PolygonShape playerShape;
		playerShape.SetAsBox(0.3f, 0.85f);
		b2FixtureDef playerFixture;
		playerFixture.density = 70.0f;
		playerFixture.shape = &playerShape;
		playerPhys.GetBody()->CreateFixture(&playerFixture);
		b2Vec2 playerFootSensorVertices[4];
		playerFootSensorVertices[0].Set(-0.2f, 0.75f);
		playerFootSensorVertices[1].Set(0.2f, 0.75f);
		playerFootSensorVertices[2].Set(0.2f, 1.05f);
		playerFootSensorVertices[3].Set(-0.2f, 1.05f);
		b2PolygonShape playerFootSensorShape;
		playerFootSensorShape.Set(playerFootSensorVertices, 4);
		playerFixture.density = 0.0f;
		playerFixture.shape = &playerFootSensorShape;
		playerFixture.isSensor = true;
		playerPhys.GetBody()->CreateFixture(&playerFixture);
		playerEntity.Add<CanJumpComponent>();
	}

	bool update(en::Time dt)
	{
		ENLIVE_PROFILE_FUNCTION();

		if (!en::Application::GetInstance().IsRunning() || !mWorld.IsPlaying())
		{
			return false;
		}

		if (getApplication().GetActionSystem().IsInputActive("close"))
		{
			getApplication().Stop();
			return false;
		}

		mWorld.Update(dt);

		return true;
	}

	void render(sf::RenderTarget& target) 
	{
		ENLIVE_PROFILE_FUNCTION();

		mWorld.Render(target);
	}

private:
	en::World mWorld;
};


//////////////////////////////////
// Main
//////////////////////////////////

int main(int argc, char** argv)
{
	en::ClassManager::Register<en::NameComponent>();
	en::ClassManager::Register<en::TransformComponent>();
	en::ClassManager::Register<en::SpriteComponent>();
	en::ClassManager::Register<en::TextComponent>();
	en::ClassManager::Register<en::RenderableComponent>();
	en::ClassManager::Register<en::UIDComponent>();
	en::ClassManager::Register<en::PhysicComponent>();

	en::ComponentManager::Register<en::NameComponent>();
	en::ComponentManager::Register<en::TransformComponent>();
	en::ComponentManager::Register<en::SpriteComponent>();
	en::ComponentManager::Register<en::TextComponent>();
	en::ComponentManager::Register<en::RenderableComponent>();
	en::ComponentManager::Register<en::UIDComponent>();
	en::ComponentManager::Register<en::PhysicComponent>();

	if (argc >= 1)
	{
		en::PathManager::GetInstance().SetExecutablePath(argv[0]);
	}

	auto& app = en::Application::GetInstance();
	app.Initialize();

	const auto vm = sf::VideoMode(1024, 768);
	auto& window = app.GetWindow();
	window.create(sf::VideoMode(1024, 768), "EngineExample", sf::Style::Titlebar | sf::Style::Close);
	window.getMainView().setSize({ vm.width * 1.0f, vm.height * 1.0f });
	window.getMainView().setCenter({ vm.width * 0.5f, vm.height * 0.5f });
	
	auto& actionSystem = app.GetActionSystem();
	actionSystem.AddInputKey("close", sf::Keyboard::Escape);
	actionSystem.AddInputKey("moveLeft", sf::Keyboard::Q, en::ActionType::Hold);
	actionSystem.AddInputKey("moveRight", sf::Keyboard::D, en::ActionType::Hold);
	actionSystem.AddInputKey("jump", sf::Keyboard::Space, en::ActionType::Pressed);

	app.Start<MyState>();

	return 0;
}
