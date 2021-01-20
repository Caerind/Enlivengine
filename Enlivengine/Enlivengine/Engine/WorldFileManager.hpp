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
	static bool SaveCurrentWorld();
	static bool UnloadCurrentWorld(bool save = false);
	static bool RemoveWorld(const std::string& worldName);
};

} // namespace en