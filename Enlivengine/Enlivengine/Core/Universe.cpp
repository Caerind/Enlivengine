#include <Enlivengine/Core/Universe.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Core/World.hpp>

namespace en
{

bool Universe::CreateWorld(const std::string& worldName)
{
	auto& universe = GetInstance();

	if (universe.mCurrentWorld != nullptr)
	{
		UnloadCurrentWorld();
	}

	enAssert(universe.mCurrentWorld == nullptr);

	universe.mCurrentWorld = new World(worldName);
	// TODO : SaveToFile
	return false;
}

bool Universe::LoadWorld(const std::string& worldName)
{
	auto& universe = GetInstance();

	if (universe.mCurrentWorld != nullptr)
	{
		UnloadCurrentWorld();
	}

	enAssert(universe.mCurrentWorld == nullptr);

	universe.mCurrentWorld = new World(worldName);
	// TODO : LoadFromFile
	return false;
}

World* Universe::GetCurrentWorld()
{
	return GetInstance().mCurrentWorld;
}

void Universe::UnloadCurrentWorld()
{
	auto& universe = GetInstance();

	enAssert(universe.mCurrentWorld != nullptr);

	delete universe.mCurrentWorld;
	// TODO : SaveToFile ?
	universe.mCurrentWorld = nullptr;
}

bool Universe::RemoveWorld(const std::string& worldName)
{
	auto& universe = GetInstance();

	if (universe.mCurrentWorld != nullptr && universe.mCurrentWorld->GetName() == worldName)
	{
		enLogError(LogChannel::Core, "Can't remove the currently loaded world {}", universe.mCurrentWorld->GetName());
		return false;
	}

	// TODO : RemoveFile
	//return std::filesystem::remove(std::filesystem::path(World::GetWorldFilename(worldName)));
	return false;
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