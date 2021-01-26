#include <Enlivengine/Core/System.hpp>

#include <Enlivengine/Core/World.hpp>

namespace en
{

System::System()
	: mWorld(nullptr)
{
}

System::~System()
{
}

void System::Update(Time dt)
{
	ENLIVE_UNUSED(dt);
}

void System::SetWorld(World* world)
{
	enAssert(world != nullptr);
	mWorld = world;
}

} // namespace en
