#pragma once

#include <Enlivengine/System/Time.hpp>

namespace sf
{
	class RenderTarget;
}

namespace en
{

class World;

class System
{
    public:
        System(World& world);
        virtual ~System();
		
        virtual void Update(Time dt);
		virtual void Render(sf::RenderTarget& target);
		
	protected:
		World& mWorld;
};
        
} // namespace en