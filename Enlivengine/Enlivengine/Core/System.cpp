#include <Enlivengine/Core/System.hpp>

#ifdef ENLIVE_MODULE_CORE

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

void System::Render()
{
}

} // namespace en

#endif // ENLIVE_MODULE_CORE
