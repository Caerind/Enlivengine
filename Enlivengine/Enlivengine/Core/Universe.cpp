#include <Enlivengine/Core/Universe.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Core/World.hpp>

namespace en
{

World* Universe::GetCurrentWorld()
{
	return GetInstance().mCurrentWorld;
}

void Universe::SetCurrentWorld(World* world)
{
	GetInstance().mCurrentWorld = world;
}

Universe& Universe::GetInstance()
{
	static Universe instance;
	return instance;
}

Universe::Universe()
	: mCurrentWorld(nullptr)
{
}

Universe::~Universe()
{
	enAssert(mCurrentWorld == nullptr);
}

} // namespace en