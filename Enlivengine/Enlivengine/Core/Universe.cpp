#include <Enlivengine/Core/Universe.hpp>

#ifdef ENLIVE_MODULE_CORE

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

#endif // ENLIVE_MODULE_CORE