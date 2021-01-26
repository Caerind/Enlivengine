#include <Enlivengine/Engine/WorldFileManager.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Universe.hpp>

#include <Enlivengine/Utils/XmlSerializer.hpp>
#include <Enlivengine/Resources/PathManager.hpp>

#include <filesystem>

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

	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + worldName + ".world");
	if (std::filesystem::exists(path))
	{
		XmlSerializer worldReader;
		if (worldReader.Open(path.string(), Serializer::Mode::Read))
		{
			if (GenericSerialization(worldReader, "World", *world))
			{
				enLogInfo(LogChannel::Core, "World {} is correctly loaded", worldName);
				return true;
			}
			else
			{
				enLogWarning(LogChannel::Core, "World {} isn't correctly loaded", worldName, path.string());
			}
		}
		else
		{
			enLogError(LogChannel::Core, "Can't open world {} : {}", worldName, path.string());
		}
	}
	else
	{
		enLogError(LogChannel::Core, "World {} doesn't exist : {}", worldName, path.string());
	}
	return false;
}

bool WorldFileManager::SaveCurrentWorld()
{
	if (World* world = Universe::GetCurrentWorld())
	{
		const std::string& worldName = world->GetName();
		const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + worldName + ".world");

		XmlSerializer worldWriter;
		if (worldWriter.Open(path.string(), Serializer::Mode::Write))
		{
			bool correctlySerialized = GenericSerialization(worldWriter, "World", *world);
			bool correctlyClosed = worldWriter.Close();

			if (correctlySerialized && correctlyClosed)
			{
				enLogInfo(LogChannel::Core, "World {} is correctly saved", worldName);
				return true;
			}
			else
			{
				enLogError(LogChannel::Core, "World {} file can't be saved : {}", worldName, path.string());
			}
		}
		else
		{
			enLogError(LogChannel::Core, "Can't save world {} : {}", worldName, path.string());
		}
	}
	else
	{
		enAssert(false);
	}
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

	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + worldName + ".world");
	if (std::filesystem::exists(path))
	{
		return std::filesystem::remove(path);
	}
	else
	{
		return true;
	}
}

} // namespace en