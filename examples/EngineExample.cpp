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

#include <Enlivengine/Application/ResourceManager.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>

#include <Enlivengine/Application/StateManager.hpp>
#include <Enlivengine/Application/Application.hpp>

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

class MyState : public en::State
{
public:
	MyState(en::StateManager& manager) : en::State(manager)
	{
		getApplication().getTextures().create("atmog", en::TextureLoader::loadFromFile("Assets/Textures/atmog.png"));
		getApplication().getTextures().create("asteroids", en::TextureLoader::loadFromFile("Assets/Textures/asteroids.png"));

		getApplication().getAudio().createMusic("mainTheme", "Assets/Musics/MainTheme.ogg");
		getApplication().getAudio().playMusic("mainTheme");

		getApplication().getAudio().createSound("click", "Assets/Sounds/click.wav");

		getApplication().getWindow().create(sf::VideoMode(1024, 768), "EngineExample");

		sprite1.setTexture(getApplication().getTextures().get("atmog"));
		sprite2.setTexture(getApplication().getTextures().get("asteroids"));
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
		}
		return true;
	}

	void render(sf::RenderTarget& target) 
	{
		target.draw(sprite1);
		target.draw(sprite2);
	}

private:
	sf::Sprite sprite1;
	sf::Sprite sprite2;
};

int main(int argc, char** argv)
{
	en::LogManager::initialize();

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

	en::Application app;
	app.start<MyState>();

	return 0;
}
