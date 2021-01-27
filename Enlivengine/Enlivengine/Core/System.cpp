#include <Enlivengine/Core/System.hpp>

#include <Enlivengine/Core/World.hpp>

namespace en
{

System::System()
	: mWorld(nullptr)
	, mFlags(static_cast<U32>(Flags::None))
{
}

System::~System()
{
}

void System::Update()
{
}

U32 System::GetFlags() const
{
	return mFlags;
}

void System::SetWorld(World* world)
{
	enAssert(world != nullptr);
	mWorld = world;
}

} // namespace en
