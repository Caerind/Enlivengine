#include <Enlivengine/Core/System.hpp>

#include <Enlivengine/Core/World.hpp>

namespace en
{

System::System()
	: mWorld(nullptr)
#ifdef ENLIVE_TOOL
	, mUpdateOnTool(false)
#endif // ENLIVE_TOOL
{
}

System::~System()
{
}

void System::Update()
{
}

#ifdef ENLIVE_TOOL
bool System::IsUpdateOnTool() const
{
	return mUpdateOnTool;
}
#endif // ENLIVE_TOOL

void System::SetWorld(World* world)
{
	enAssert(world != nullptr);
	mWorld = world;
}

} // namespace en
