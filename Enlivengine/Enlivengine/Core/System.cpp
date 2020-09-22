#include "System.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <Enlivengine/Core/World.hpp>

namespace en
{

System::System(World& world)
	: mWorld(world)
{
}

System::~System()
{
}

void System::Update(Time dt)
{
	ENLIVE_UNUSED(dt);
}

void System::Render(sf::RenderTarget& target)
{
	ENLIVE_UNUSED(target);
}

} // namespace en
