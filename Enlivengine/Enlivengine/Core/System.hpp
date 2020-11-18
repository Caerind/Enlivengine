#pragma once

#include <Enlivengine/Platform/Time.hpp>

namespace en
{

class World;

class System
{
    public:
        System(World& world);
        virtual ~System();
		
        virtual void Update(Time dt);
		virtual void Render();
		
	protected:
		World& mWorld;
};
        
} // namespace en