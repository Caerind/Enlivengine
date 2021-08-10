#pragma once

#include <string>

namespace en
{

class World;

class Universe
{
public:
	static World* GetCurrentWorld();

private:
	friend class WorldFileManager;
	static void SetCurrentWorld(World* world);

private:
	static Universe& GetInstance();

	Universe();
	~Universe();

private:
	World* mCurrentWorld;
};

} // namespace en