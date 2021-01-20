#pragma once

#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Utils/Meta.hpp>

namespace en
{

class World;

class System
{
    public:
        System();
        virtual ~System();
		
        virtual void Update(Time dt);
		virtual void Render();

		virtual const char* GetName() const = 0;
		
	protected:
		World* mWorld;

	private:
		friend class World;
		void SetWorld(World* world);
};
        
} // namespace en

ENLIVE_META_CLASS_BEGIN(en::System, true, true)
ENLIVE_META_CLASS_END()