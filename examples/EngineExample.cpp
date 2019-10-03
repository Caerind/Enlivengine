#include <Enlivengine/System/Macros.hpp>
#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/CompilerDetection.hpp>
#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/ByteUnits.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Endianness.hpp>
#include <Enlivengine/System/Debugger.hpp>
#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/System/Hash.hpp>

#include <Enlivengine/Math/Random.hpp>

#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Graphics/DebugDraw.hpp>

#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Application/StateManager.hpp>
#include <Enlivengine/Application/Application.hpp>

#include <Enlivengine/Core/Components.hpp>

#include <Box2D/Box2D.h>

#include <entt/entt.hpp>

#include <imgui/imgui.h>

#include <SFML/Graphics.hpp>


class MyState : public en::State
{
public:
	MyState(en::StateManager& manager) : en::State(manager)
	{
		getApplication().getWindow().setMainView(en::View(0.f, 0.f, 1024.f, 768.f));

		getApplication().getTextures().create("atmog", en::TextureLoader::loadFromFile("Assets/Textures/atmog.png"));
		asteroidTextureId = getApplication().getTextures().create("asteroids", en::TextureLoader::loadFromFile("Assets/Textures/asteroids.png"));
		fontId = getApplication().getFonts().create("font", en::FontLoader::loadFromFile("Assets/Fonts/ErasBoldITC.ttf"));

		getApplication().getAudio().createMusic("mainTheme", "Assets/Musics/MainTheme.ogg");
		getApplication().getAudio().playMusic("mainTheme");

		getApplication().getAudio().createSound("click", "Assets/Sounds/click.wav");

		getApplication().getWindow().create(sf::VideoMode(1024, 768), "EngineExample");

		background.setTexture(getApplication().getTextures().get("atmog"));
	}

	bool handleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::Closed)
		{
			getApplication().stop();
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			getApplication().getAudio().playSound("click");
			createEntity(getApplication().getWindow().getCursorPosition().x, getApplication().getWindow().getCursorPosition().y);
		}
		if (event.type == sf::Event::KeyPressed)
		{
#ifdef ENLIVE_ENABLE_IMGUI
			if (event.key.code == sf::Keyboard::F3)
			{
				mShowMetrics = !mShowMetrics;
			}
#endif
		}
		return true;
	}

	bool update(en::Time dt)
	{
#ifdef ENLIVE_ENABLE_IMGUI
		if (mShowMetrics)
		{
			ImGui::Begin("Metrics");
			ImGui::Text("Entities : %d", registry.size());
			ImGui::Text("Mouse X : %d", (int)getApplication().getWindow().getCursorPosition().x);
			ImGui::Text("Mouse Y : %d", (int)getApplication().getWindow().getCursorPosition().y);
			ImGui::Text("View X : %d", (int)getApplication().getWindow().getMainView().getCenter().x);
			ImGui::Text("View Y : %d", (int)getApplication().getWindow().getMainView().getCenter().y);
#ifdef ENLIVE_DEBUG
			ImGui::Text("DebugDisplay Rectangles : %d", en::DebugDraw::getCurrentRectangleCount());
			ImGui::Text("DebugDisplay Circles : %d", en::DebugDraw::getCurrentCircleCount());
#endif // ENLIVE_DEBUG
			ImGui::End();
		}
#endif

		return true;
	}

	void render(sf::RenderTarget& target) 
	{
		registry.sort<en::RenderableComponent>([](const auto& lhs, const auto& rhs) 
		{
			return lhs.z < rhs.z;
		});

		target.draw(background);

		auto view = registry.view<en::RenderableComponent, en::PositionComponent>();
		for (auto entity : view) 
		{
			const auto& renderable = view.get<en::RenderableComponent>(entity);
			const auto& position = view.get<en::PositionComponent>(entity);

			sf::RenderStates states;
			states.transform = position.getTransform();

			if (registry.has<en::SpriteComponent>(entity))
			{
				target.draw(registry.get<en::SpriteComponent>(entity), states);
			}
			if (registry.has<en::AnimationComponent>(entity))
			{

			}
			if (registry.has<en::TextComponent>(entity))
			{
				target.draw(registry.get<en::TextComponent>(entity), states);
			}
		}
	}

	void createEntity(en::F32 x, en::F32 y)
	{
		auto entity = registry.create();
		auto& position = registry.assign<en::PositionComponent>(entity);
		position.setPosition(x, y);
		auto& renderable = registry.assign<en::RenderableComponent>(entity);
		renderable.z = 0;
		auto& sprite = registry.assign<en::SpriteComponent>(entity);
		sprite.setTexture(getApplication().getTextures().get(asteroidTextureId));
		sprite.setTextureRect(en::toSF(en::Recti(0, 0, 120, 120)));
		sprite.setOrigin(60, 60);
		auto& text = registry.assign<en::TextComponent>(entity);
		text.setFont(getApplication().getFonts().get(fontId));
		text.setPosition(10, 10);
		text.setString("Asteroid");
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1.f);
	}

private:
	entt::registry registry;
	sf::Sprite background;
	en::U32 asteroidTextureId;
	en::U32 fontId;

#ifdef ENLIVE_ENABLE_IMGUI
	bool mShowMetrics;
#endif
};

int main(int argc, char** argv)
{
	en::LogManager::initialize();

	en::Application app; // Moved this before just to see the logs inside ImGui

	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	LogInfo(en::LogChannel::System, 9, "%4.2f %4.2f %4.2f", body->GetPosition().x, body->GetPosition().y, body->GetAngle());
	for (int32 i = 0; i < 20; ++i)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
	}
	LogInfo(en::LogChannel::System, 9, "%4.2f %4.2f %4.2f", body->GetPosition().x, body->GetPosition().y, body->GetAngle());
	for (int32 i = 0; i < 20; ++i)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
	}
	LogInfo(en::LogChannel::System, 9, "%4.2f %4.2f %4.2f", body->GetPosition().x, body->GetPosition().y, body->GetAngle());
	for (int32 i = 0; i < 20; ++i)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
	}
	LogInfo(en::LogChannel::System, 9, "%4.2f %4.2f %4.2f", body->GetPosition().x, body->GetPosition().y, body->GetAngle());

	LogWarning(en::LogChannel::System, 9, "%4.2f %4.2f %4.2f", body->GetPosition().x, body->GetPosition().y, body->GetAngle());

	LogError(en::LogChannel::System, 9, "%4.2f %4.2f %4.2f", body->GetPosition().x, body->GetPosition().y, body->GetAngle());

	app.start<MyState>();

	return 0;
}
