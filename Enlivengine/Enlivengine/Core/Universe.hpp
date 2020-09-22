#pragma once

#include <Enlivengine/Core/World.hpp>

namespace en
{

class Universe
{
	ENLIVE_SINGLETON(Universe);
	~Universe();

public:
	void SetCurrentWorld(World* world);
	World* GetCurrentWorld();
	const World* GetCurrentWorld() const;

private:
	World* mCurrentWorld;
};

} // namespace en