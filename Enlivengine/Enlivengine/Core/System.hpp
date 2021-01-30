#pragma once

#include <Enlivengine/Platform/Time.hpp>
#include <Enlivengine/Utils/Meta.hpp>

namespace en
{

class World;
class SystemManager;

class System
{
    public:
        System();
        virtual ~System();
		
        virtual void Update();

		virtual const char* GetName() const = 0;
		virtual bool Serialize(Serializer& serializer, const char* name) = 0;
		virtual bool Edit(ObjectEditor& objectEditor, const char* name) = 0;

#ifdef ENLIVE_TOOL
		bool IsUpdateOnTool() const;
#endif // ENLIVE_TOOL

	protected:
		World* mWorld;

#ifdef ENLIVE_TOOL
		bool mUpdateOnTool;
#endif // ENLIVE_TOOL

	private:
		friend class World;
		friend class SystemManager;
		void SetWorld(World* world);
};
        
} // namespace en

ENLIVE_META_CLASS_BEGIN(en::System, en::Type_VirtualSerialization, en::Type_VirtualEditor)
ENLIVE_META_CLASS_END()