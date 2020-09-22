#include <Enlivengine/Core/Universe.hpp>

namespace en
{

Universe::Universe()
	: mCurrentWorld(nullptr)
{
}

Universe::~Universe()
{
}

void Universe::SetCurrentWorld(World* world)
{
	mCurrentWorld = world;
}

World* Universe::GetCurrentWorld()
{
	return mCurrentWorld;
}

const World* Universe::GetCurrentWorld() const
{
	return mCurrentWorld;
}

} // namespace en