#pragma once

#include <string>

namespace en
{

class World;

class WorldFileManager
{
public:
	WorldFileManager() = delete;

	static bool CreateWorld(const std::string& worldName);
	static bool LoadWorld(const std::string& worldName);
	static bool LoadWorldTransition(const std::string& worldName);
	static bool SaveCurrentWorld();
	static bool UnloadCurrentWorld();
	static bool RemoveWorld(const std::string& worldName);

private:
	static World* LoadWorld_Internal(const std::string& worldName);
	static bool SaveWorld_Internal(World* world);
	static void UnloadWorld_Internal(World* world);
};

} // namespace en