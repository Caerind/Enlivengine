#pragma once

#include <string>

namespace en
{

class World;

class Universe
{
public:
	static bool CreateWorld(const std::string& worldName);
	static bool LoadWorld(const std::string& worldName);
	static World* GetCurrentWorld();
	static void UnloadCurrentWorld();
	static bool RemoveWorld(const std::string& worldName);

private:
	static Universe& GetInstance();

	Universe();
	~Universe();

private:
	World* mCurrentWorld;
};

} // namespace en