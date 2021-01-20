#include <Enlivengine/Engine/WorldFileManager.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Universe.hpp>

namespace en
{

bool WorldFileManager::CreateWorld(const std::string& worldName)
{
	if (Universe::GetCurrentWorld() != nullptr)
	{
		UnloadCurrentWorld();
	}
	enAssert(Universe::GetCurrentWorld() == nullptr);

	World* world = new World(worldName);
	Universe::SetCurrentWorld(world);
	enAssert(world != nullptr);

	return SaveCurrentWorld();
}

bool WorldFileManager::LoadWorld(const std::string& worldName)
{
	if (Universe::GetCurrentWorld() != nullptr)
	{
		UnloadCurrentWorld();
	}
	enAssert(Universe::GetCurrentWorld() == nullptr);

	World* world = new World(worldName);
	Universe::SetCurrentWorld(world);
	enAssert(world != nullptr);

	// TODO : LoadFromFile
	return false;
}

bool WorldFileManager::SaveCurrentWorld()
{
	World* world = Universe::GetCurrentWorld();
	enAssert(world != nullptr);

	// TODO : SaveToFile
	return false;
}

bool WorldFileManager::UnloadCurrentWorld(bool save /*= false*/)
{
	World* world = Universe::GetCurrentWorld();
	enAssert(world != nullptr);

	bool correctlySaved = true; // If we don't want to save, assume we correctly saved
	if (save)
	{
		correctlySaved = SaveCurrentWorld();
	}

	delete world;
	Universe::SetCurrentWorld(nullptr);

	return correctlySaved;
}

bool WorldFileManager::RemoveWorld(const std::string& worldName)
{
	World* world = Universe::GetCurrentWorld();
	if (world != nullptr && world->GetName() == worldName)
	{
		enLogError(LogChannel::Core, "Can't remove the currently loaded world {}", world->GetName());
		return false;
	}

	// TODO : RemoveFile
	//return std::filesystem::remove(std::filesystem::path(World::GetWorldFilename(worldName)));
	return false;
}

} // namespace en