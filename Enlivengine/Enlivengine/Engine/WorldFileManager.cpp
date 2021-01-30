#include <Enlivengine/Engine/WorldFileManager.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Core/World.hpp>
#include <Enlivengine/Core/Universe.hpp>

#include <Enlivengine/Utils/XmlSerializer.hpp>
#include <Enlivengine/Resources/PathManager.hpp>

#ifdef ENLIVE_TOOL
#include <Enlivengine/Tools/ImGuiEditor.hpp>
#endif // ENLIVE_TOOL

#include <filesystem>

namespace en
{

bool WorldFileManager::CreateWorld(const std::string& worldName)
{
	UnloadCurrentWorld();

	World* world = new World(worldName);
	Universe::SetCurrentWorld(world);
	enAssert(world != nullptr);

	return SaveCurrentWorld();
}

bool WorldFileManager::LoadWorld(const std::string& worldName)
{
	UnloadCurrentWorld(); // Unload the current world first

	if (World* world = LoadWorld_Internal(worldName))
	{
		Universe::SetCurrentWorld(world);
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldFileManager::LoadWorldTransition(const std::string& worldName)
{
	if (World* world = LoadWorld_Internal(worldName))
	{
		UnloadCurrentWorld(); // Unload the current world only if we succeed to load the new world

		Universe::SetCurrentWorld(world);
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldFileManager::SaveCurrentWorld()
{
#ifdef ENLIVE_TOOL
	enAssert(ImGuiEditor::IsStopped());
#endif // ENLIVE_TOOL

	if (World* world = Universe::GetCurrentWorld())
	{
		return SaveWorld_Internal(world);
	}
	else
	{
		return false;
	}
}

bool WorldFileManager::UnloadCurrentWorld()
{
	if (World* world = Universe::GetCurrentWorld())
	{
		UnloadWorld_Internal(world);
		return true;
	}
	else
	{
		return false;
	}
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

World* WorldFileManager::LoadWorld_Internal(const std::string& worldName)
{
	World* world = new World(worldName);
	if (world == nullptr)
	{
		return nullptr;
	}

	const std::filesystem::path path = std::string(PathManager::GetAssetsPath() + worldName + ".world");
	if (std::filesystem::exists(path))
	{
		XmlSerializer worldReader;
		if (worldReader.Open(path.string(), Serializer::Mode::Read))
		{
			if (GenericSerialization(worldReader, "World", *world))
			{
				enLogInfo(LogChannel::Core, "World {} is correctly loaded", worldName);
			}
			else
			{
				enLogWarning(LogChannel::Core, "World {} isn't correctly loaded", worldName, path.string());
			}
			return world;
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
	return nullptr;
}

bool WorldFileManager::SaveWorld_Internal(World* world)
{
	enAssert(world != nullptr);

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
			return false;
		}
	}
	else
	{
		enLogError(LogChannel::Core, "Can't save world {} : {}", worldName, path.string());
		return false;
	}
}

void WorldFileManager::UnloadWorld_Internal(World* world)
{
	enAssert(world != nullptr);

	delete world;
	Universe::SetCurrentWorld(nullptr);
}

} // namespace en